/**
 * @file   drGridAstar.cpp
 * @brief  Detailed Routing - Grid-Based A* search kernel
 * @author Hao Chen
 * @date   02/05/2020
 *
 **/

#include "drGridAstar.hpp"
#include "src/ds/pqueue.hpp"
#include "src/geo/kdtree.hpp"
#include "src/acs/acsMgr.hpp"
#include "src/geo/box2polygon.hpp"

#include <lemon/maps.h>
#include <lemon/kruskal.h>
#include <lemon/list_graph.h>

PROJECT_NAMESPACE_START

bool DrGridAstar::run() {
  
  if (_net.bRouted())
    return true;

  init();
  splitSubNetMST();

  // route net with sym and self-sym constraint
  bool bSuccess = route();

  // ripup or not
  if (!bSuccess) {
    ripup();
    return false;
  }
  saveResult2Net();
  return true;
}

void DrGridAstar::init() {
  _compDS.init(_net.numPins());
  _vCompBoxes.resize(_net.numPins());
  _vCompAcsPts.resize(_net.numPins());
  _vCompSpatialBoxes.resize(_net.numPins());

  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  const AcsPt* cpPt;
  Net_ForEachPinIdx(_net, pinIdx, i) {
    _vCompAcsPts[i].set_empty_key(Point3d<Int_t>(MIN_INT, MIN_INT, MIN_INT));
    _vCompAcsPts[i].set_deleted_key(Point3d<Int_t>(MAX_INT, MAX_INT, MAX_INT));
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, j) {
        _vCompBoxes[i].emplace_back((*cpBox), layerIdx);
        _vCompSpatialBoxes[i][layerIdx].insert(*cpBox);
      }
    }

    Pin_ForEachAcsPt(pin, cpPt, j) {
      Point3d<Int_t> pt = cpPt->gridPt();
      //switch (cpPt->dir()) {
        //case AcsPt::DirType::NORTH: pt.setY(pt.y() + _cir.gridStep()); break;
        //case AcsPt::DirType::SOUTH: pt.setY(pt.y() - _cir.gridStep()); break;
        //case AcsPt::DirType::EAST: pt.setX(pt.x() + _cir.gridStep()); break;
        //case AcsPt::DirType::WEST: pt.setX(pt.x() - _cir.gridStep()); break;
        //case AcsPt::DirType::UP: assert(false);
        //case AcsPt::DirType::DOWN: assert(false);
        //default: assert(false);
      //}
      _vCompAcsPts[i].insert(pt);
      _pinAcsMap[pt] = *cpPt;
    }
  }
}

void DrGridAstar::splitSubNetMST() {
  // init graph, vertex and edge sets
  lemon::ListGraph graph;
  Vector_t<lemon::ListGraph::Node> vGraphNodes;
  Vector_t<lemon::ListGraph::Edge> vGraphEdges;
  lemon::ListGraph::EdgeMap<Int_t> edgeCostMap(graph);

  // compute cost value of each edge
  Vector_t<Int_t> vEdgeCostValues;
  UInt_t i, j;
  for (i = 0; i < _vCompBoxes.size(); ++i) {
    vGraphNodes.emplace_back(graph.addNode());
  }
  for (i = 0; i < _vCompBoxes.size(); ++i) {
    for (j = i + 1; j < _vCompBoxes.size(); ++j) {
      vGraphEdges.emplace_back(graph.addEdge(vGraphNodes[i], vGraphNodes[j]));
      Int_t minDist = MAX_INT;
      for (const auto& u : _vCompBoxes[i]) {
        for (const auto& v : _vCompBoxes[j]) {
          Int_t dist = scaledMDist(u, v);
          if (dist < minDist) {
            minDist = dist;
          }
        }
      }
      assert(minDist != MAX_INT);
      vEdgeCostValues.emplace_back(minDist);
    }
  }
  assert(vEdgeCostValues.size() == vGraphEdges.size()); 
  for (i = 0; i < vGraphEdges.size(); ++i) {
    edgeCostMap.set(vGraphEdges[i], vEdgeCostValues[i]);
  }

  // solve graph MST
  Vector_t<lemon::ListGraph::Edge> vResEdges;
  lemon::kruskal(graph, edgeCostMap, std::back_inserter(vResEdges));
  assert(vResEdges.size() == _vCompBoxes.size() - 1); // |E| == |V| - 1
  
  // save the MST result
  for (i = 0; i < vResEdges.size(); ++i) {
    lemon::ListGraph::Edge& edge = vResEdges[i];
    _vSubNets.emplace_back(graph.id(graph.u(edge)), graph.id(graph.v(edge)));
  }
}

bool DrGridAstar::route() {
  for (const auto& pair : _vSubNets) {
    const Int_t srcIdx = pair.first;
    const Int_t tarIdx = pair.second;
    if (!routeSubNet(srcIdx, tarIdx)) {
      fprintf(stderr, "DrGridAstar::%s ERROR: Route net %s failed!\n", __func__, _net.name().c_str());
      return false;
    }
  }
  return true;
}

bool DrGridAstar::routeSubNet(Int_t srcIdx, Int_t tarIdx) {
  assert(_compDS.find(srcIdx) != _compDS.find(tarIdx));
  
  // init source and target access point sets
  srcIdx = _compDS.find(srcIdx);
  tarIdx = _compDS.find(tarIdx);
  auto& src = _vCompAcsPts[srcIdx]; // DenseHashSet
  auto& tar = _vCompAcsPts[tarIdx]; // DenseHashSet

  // reset nodes cost
  resetAllNodes();

  // init access points to node maps
  // memory free in destructor
  for (const auto& p : src) { // p: Point3d<Int>
    Point<Int_t> p2d(p.x(), p.y());
    if (_vAllNodesMap[p.z()].find(p2d) == _vAllNodesMap[p.z()].end()) {
      _vAllNodesMap[p.z()][p2d] = new DrGridAstarNode(p);
    }
  }
  for (const auto& p : tar) { // p: Point3d<Int>
    Point<Int_t> p2d(p.x(), p.y());
    if (_vAllNodesMap[p.z()].find(p2d) == _vAllNodesMap[p.z()].end()) {
      _vAllNodesMap[p.z()][p2d] = new DrGridAstarNode(p);
    }
  }

  return pathSearch(srcIdx, tarIdx);
}

 bool DrGridAstar::pathSearch(const Int_t srcIdx, const Int_t tarIdx) {
  auto& src = _vCompAcsPts[srcIdx]; // DenseHashSet
  auto& tar = _vCompAcsPts[tarIdx]; // DenseHashSet
  
  // init KD-Tree of tar
  K3dTree<Int_t> tarKD;
  for (const auto& p : tar) {
    tarKD.insert(p.x() * _param.horCost,
                 p.y() * _param.verCost,
                 p.z() * _param.viaCost);
  }
  tarKD.buildIndex();
  
  // init Astar priority queue
  using PQueue_t = PairingHeap<DrGridAstarNode*, DrGridAstarNodeCmp>;
  using Itermap_t = DenseHashMap<DrGridAstarNode*, PQueue_t::point_iterator>;
  PQueue_t pq;
  Itermap_t itMap;
  itMap.set_empty_key(nullptr);

  // add src points to pq
  for (const auto& p : src) {
    Point<Int_t> p2d(p.x(), p.y());
    Int_t layerIdx = p.z();
    Point3d<Int_t> scaledP(p.x() * _param.horCost,
                           p.y() * _param.verCost,
                           p.z() * _param.viaCost);
    Point3d<Int_t> scaledNearestP;
    Int_t scaledNearestDist = MAX_INT;
    tarKD.nearestSearch(scaledP, scaledNearestP, scaledNearestDist);
    assert(_vAllNodesMap[layerIdx].find(p2d) != _vAllNodesMap[layerIdx].end());
    DrGridAstarNode* pNode = _vAllNodesMap[layerIdx][p2d];
    Int_t costF = _param.factorH * scaledNearestDist;
    if (bInsideGuide(pNode)) {
      costF += _param.guideCost;
    }
    pNode->setCostF(costF);
    pNode->setCostG(0);
    pNode->setBendCnt(0);
    //pNode->setDist2Tar()
    itMap[pNode] = pq.push(pNode);
  }
  
  // start exploring nodes
  while (!pq.empty()) {
    DrGridAstarNode* pU = pq.top();
    if (bTerminate(pU, tarIdx)) { // u \in tar
      const UInt_t bigCompIdx = mergeComp(srcIdx, tarIdx);
      //connect2AcsPt(pU);
      backTrack(pU, bigCompIdx, srcIdx, tarIdx);
      return true;
    }
    pq.pop();
    pU->setExplored(true);
    if (pU->vpNeighbors().empty()) {
      findNeighbors(pU);
    }
    for (auto pV : pU->vpNeighbors()) {
      if (pV->bExplored())
        continue;
      const Int_t costG = pU->costG() + scaledMDist(pU->coord(), pV->coord());
      const Int_t bendCnt = pU->bendCnt() + hasBend(pU, pV);
      if (bNeedUpdate(pV, costG, bendCnt)) {
        pV->setParent(pU);
        Point3d<Int_t> scaledP(pV->coord().x() * _param.horCost,
                               pV->coord().y() * _param.verCost,
                               pV->coord().z() * _param.viaCost);
        Point3d<Int_t> scaledNearestP;
        Int_t scaledNearestDist;
        tarKD.nearestSearch(scaledP, scaledNearestP, scaledNearestDist);
        Int_t costF = (costG * _param.factorG + scaledNearestDist * _param.factorH);
        if (bViolateDRC(pU, pV)) {
          if (_bStrictDRC) // do not allow any DRC violation
            continue;
          else // add high cost to DRC violations
            costF += _param.drcCost;
        }
        if (bInsideGuide(pV)) {
          costF += _param.guideCost;
        }
        costF += history(pV);
        pV->setCostF(costF);
        pV->setCostG(costG);
        pV->setBendCnt(bendCnt);
        auto it = itMap.find(pV);
        if (it != itMap.end())
          pq.modify(it->second, pV);
        else
          itMap[pV] = pq.push(pV);
      }
    }
  }
  return false;
}

bool DrGridAstar::bTerminate(DrGridAstarNode* pU, const Int_t tarIdx) {
  const auto& tar = _vCompAcsPts[tarIdx];
  if (tar.find(pU->coord()) != tar.end()) {
    //const AcsPt& acsPt = _pinAcsMap.at(pU->coord());
    //if (findAcsDir(pU) == acsPt.dir())
      return true;
  }
  return false;
}

Int_t DrGridAstar::mergeComp(const Int_t srcIdx, const Int_t tarIdx) {
  _compDS.merge(srcIdx, tarIdx);
  Int_t bigCompIdx = srcIdx, smallCompIdx = tarIdx;
  if (_vCompAcsPts[bigCompIdx].size() < _vCompAcsPts[smallCompIdx].size()) {
    std::swap(bigCompIdx, smallCompIdx);
  }
  auto& bigComp = _vCompAcsPts[bigCompIdx];
  auto& smallComp = _vCompAcsPts[smallCompIdx];
  if (_compDS.nSets() > 1) {
    bigComp.insert(smallComp.begin(), smallComp.end());
  }
  return bigCompIdx;
}

void DrGridAstar::backTrack(const DrGridAstarNode* pU, const Int_t bigCompIdx, const Int_t srcIdx, const Int_t tarIdx) {
  List_t<Point3d<Int_t>> lPathPts;
  add2Path(pU, lPathPts);
  const DrGridAstarNode* pParent = pU->pParent();
  if (pParent == nullptr)
    return;
  while (pParent != nullptr) {
    add2Path(pParent, lPathPts);
    pParent = pParent->pParent();
  }
  assert(_compDS.find(srcIdx) == _compDS.find(tarIdx));
  const Int_t rootIdx = _compDS.find(srcIdx);
  const Int_t childIdx = (rootIdx == srcIdx) ? tarIdx : srcIdx;
  _vCompAcsPts[rootIdx] = _vCompAcsPts[bigCompIdx];
  List_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>> lPathVec;
  mergePath(lPathPts, lPathVec);
  
  savePath(lPathVec);
  

  // add to comp box
  for (const auto& pair : _vCompBoxes[childIdx]) {
    _vCompBoxes[rootIdx].emplace_back(pair);
    _vCompSpatialBoxes[rootIdx][pair.second].insert(pair.first);
  }
  const auto& vRoutePath = _vvRoutePaths.back();
  for (const auto& vec : vRoutePath) {
    const auto& u = vec.first;
    const auto& v = vec.second;
    if (u.z() == v.z()) {
      // get the layer information
      assert(_cir.lef().bRoutingLayer(u.z()));
      const auto& layerPair = _cir.lef().layerPair(u.z());
      const auto& layer = _cir.lef().routingLayer(layerPair.second);
      const Int_t width = layer.minWidth();
      const Int_t extension = layer.minWidth() / 2;
      // generate the exact wire shape
      Box<Int_t> wire;
      toWire(u, v, width, extension, wire);
      addAcsPts(rootIdx, u.z(), wire);
      _vCompBoxes[rootIdx].emplace_back(wire, u.z());
      _vCompSpatialBoxes[rootIdx][u.z()].insert(wire);
    }
  }
}

void DrGridAstar::add2Path(const DrGridAstarNode* pU, List_t<Point3d<Int_t>>& lPathPts) {
  lPathPts.emplace_front(pU->coord()); 
}

void DrGridAstar::mergePath(const List_t<Point3d<Int_t>>& lPathPts, List_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>& lPathVec) {
  if (lPathPts.size() == 1) {
    lPathVec.emplace_back(lPathPts.front(), lPathPts.front());
    return;
  }
  auto u1 = lPathPts.begin();
  auto v1 = std::next(u1, 1);
  auto v2 = std::next(v1, 1);
  for (; v2 != lPathPts.end(); ++v2) {
    if (bNeedMergePath(*u1, *v1, *v1, *v2)) {
      v1 = v2;
    }
    else {
      lPathVec.emplace_back(*u1, *v1);
      u1 = v1;
      v1 = v2;
    }
  }
  lPathVec.emplace_back(*u1, *v1);
}

bool DrGridAstar::bNeedMergePath(const Point3d<Int_t>& u1, const Point3d<Int_t>& v1, const Point3d<Int_t>& u2, const Point3d<Int_t>& v2) {
  // path1: u1 -> v1, path2: u2 -> v2
  if (u1.z() != v1.z()) {
    assert(u1.x() == v1.x() and u1.y() == v1.y());
    return false;
  }
  return findDir(u1, v1) == findDir(u2, v2);
}

void DrGridAstar::savePath(const List_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>& lPathVec) {
  _vvRoutePaths.resize(_vvRoutePaths.size() + 1);
  _vvRoutePaths.back().reserve(lPathVec.size());
  _vvRoutedWires.resize(_vvRoutedWires.size() + 1);
  _vvRoutedWires.back().reserve(lPathVec.size());
  for (const auto& pair : lPathVec) {
    _vvRoutePaths.back().emplace_back(pair);
  }
  // add exact shapes to spatial
  const auto& vPathVec = _vvRoutePaths.back();
  auto& vRoutedWires = _vvRoutedWires.back();
  for (const auto& pair : vPathVec) {
    const auto& u = pair.first;
    const auto& v = pair.second;
    if (u.z() == v.z()) {
      // get the layer information
      assert(_cir.lef().bRoutingLayer(u.z()));
      const auto& layerPair = _cir.lef().layerPair(u.z());
      const auto& layer = _cir.lef().routingLayer(layerPair.second);
      const Int_t width = layer.minWidth();
      const Int_t extension = layer.minWidth() / 2;
      // generate the exact wire
      Box<Int_t> wire;
      toWire(u, v, width, extension, wire);
      vRoutedWires.emplace_back(wire, u.z());
      _cir.addSpatialRoutedWire(_net.idx(), u.z(), wire);
      // add symmetric wire to spatial routed wire, for DRC
      if (_bSym) {
        Box<Int_t> symWire(wire);
        symWire.flipX(_cir.symAxisX());
        _cir.addSpatialRoutedWire(_net.symNetIdx(), u.z(), symWire);
      }
      // add history cost
      _dr.addWireHistoryCost(_param.historyCost, u.z(), wire);
    }
    else {
      // choose via
      assert(u.x() == v.x() and u.y() == v.y());
      const Int_t x = u.x();
      const Int_t y = u.y();
      const Int_t botLayerIdx = std::min(u.z(), v.z());
      const LefVia& via = _cir.lef().via(botLayerIdx, 1, 1);
      via2LayerBoxes(x, y, via, vRoutedWires);     
      _cir.addSpatialRoutedVia(_net.idx(), x, y, via);
      // add symmetric via to spatial routed wire, for DRC
      if (_bSym) {
        const Int_t symX = 2 * _cir.symAxisX() - x;
        _cir.addSpatialRoutedVia(_net.idx(), symX, y, via);
      }
      // add history cost
      _dr.addViaHistoryCost(_param.historyCost, x, y, via);
    }
  }
}

void DrGridAstar::via2LayerBoxes(const Int_t x, const Int_t y, const LefVia& via, Vector_t<Pair_t<Box<Int_t>, Int_t>>& vLayerBoxes) {
  for (auto box : via.vBotBoxes()) {
    box.shift(x, y);
    vLayerBoxes.emplace_back(box, via.botLayerIdx());
  }
  for (auto box : via.vCutBoxes()) {
    box.shift(x, y);
    vLayerBoxes.emplace_back(box, via.cutLayerIdx());
  }
  for (auto box : via.vTopBoxes()) {
    box.shift(x, y);
    vLayerBoxes.emplace_back(box, via.topLayerIdx());
  }
}

void DrGridAstar::addAcsPts(const Int_t idx, const Int_t z, const Box<Int_t>& box) {
  Vector_t<Point3d<Int_t>> vAcs;
  AcsMgr acs(_cir);
  acs.computeBoxAcs(box, z, vAcs);
  _vCompAcsPts[idx].insert(vAcs.begin(), vAcs.end());
}

void DrGridAstar::findNeighbors(DrGridAstarNode* pU) {
  Vector_t<DrGridAstarNode*> vpNeighbors;
  const auto& p = pU->coord();
  assert(p != Point3d<Int_t>(0, 0, 0));
  assert(_cir.lef().bRoutingLayer(p.z()));
  
  // set step size
  const Int_t step = _cir.gridStep();

  // find lower layer neighbor
  if (p.z() > 0) {
    const Point<Int_t> p2d(p.x(), p.y());
    const Int_t layerIdx = p.z() - 2;
    if (layerIdx >= 0 and _cir.lef().bRoutingLayer(layerIdx)) {
      // add new node if not visited before
      if (_vAllNodesMap[layerIdx].find(p2d) == _vAllNodesMap[layerIdx].end()) {
        _vAllNodesMap[layerIdx][p2d] = new DrGridAstarNode(Point3d<Int_t>(p.x(), p.y(), layerIdx));
      }
      pU->addNeighbor(_vAllNodesMap[layerIdx][p2d]);
    }
  }
  // find upper layer neighbor
  if (p.z() < (Int_t)_vAllNodesMap.size() - 2) {
    const Point<Int_t> p2d(p.x(), p.y());
    const Int_t layerIdx = p.z() + 2;
    if (_cir.lef().bRoutingLayer(layerIdx)) {
      // add new node if not visited before
      if (_vAllNodesMap[layerIdx].find(p2d) == _vAllNodesMap[layerIdx].end()) {
        _vAllNodesMap[layerIdx][p2d] = new DrGridAstarNode(Point3d<Int_t>(p.x(), p.y(), layerIdx));
      }
      pU->addNeighbor(_vAllNodesMap[layerIdx][p2d]);
    }
  }
  // find left neighbor
  if (p.x() > _cir.xl()) {
    const Int_t layerIdx = p.z();
    const Int_t newX = p.x() - step;
    const Point<Int_t> neighborP(newX, p.y());
    if (_vAllNodesMap[layerIdx].find(neighborP) == _vAllNodesMap[layerIdx].end()) {
      _vAllNodesMap[layerIdx][neighborP] = new DrGridAstarNode(Point3d<Int_t>(neighborP.x(), neighborP.y(), layerIdx));
    }
    pU->addNeighbor(_vAllNodesMap[layerIdx][neighborP]);
  }
  // find right neighbor
  if (p.x() < _cir.xh()) {
    const Int_t layerIdx = p.z();
    const Int_t newX = p.x() + step;
    const Point<Int_t> neighborP(newX, p.y());
    if (_vAllNodesMap[layerIdx].find(neighborP) == _vAllNodesMap[layerIdx].end()) {
      _vAllNodesMap[layerIdx][neighborP] = new DrGridAstarNode(Point3d<Int_t>(neighborP.x(), neighborP.y(), layerIdx));
    }
    pU->addNeighbor(_vAllNodesMap[layerIdx][neighborP]);
  }
  // find down neighbor
  if (p.y() > _cir.yl()) {
    const Int_t layerIdx = p.z();
    const Int_t newY = p.y() - step;
    const Point<Int_t> neighborP(p.x(), newY);
    if (_vAllNodesMap[layerIdx].find(neighborP) == _vAllNodesMap[layerIdx].end()) {
      _vAllNodesMap[layerIdx][neighborP] = new DrGridAstarNode(Point3d<Int_t>(neighborP.x(), neighborP.y(), layerIdx));
    }
    pU->addNeighbor(_vAllNodesMap[layerIdx][neighborP]);
  }
  // find up neighbor
  if (p.y() < _cir.yh()) {
    const Int_t layerIdx = p.z();
    const Int_t newY = p.y() + step;
    const Point<Int_t> neighborP(p.x(), newY);
    if (_vAllNodesMap[layerIdx].find(neighborP) == _vAllNodesMap[layerIdx].end()) {
      _vAllNodesMap[layerIdx][neighborP] = new DrGridAstarNode(Point3d<Int_t>(neighborP.x(), neighborP.y(), layerIdx));
    }
    pU->addNeighbor(_vAllNodesMap[layerIdx][neighborP]);
  }
}

bool DrGridAstar::bViolateDRC(const DrGridAstarNode* pU, const DrGridAstarNode* pV) {
  const auto& u = pU->coord();
  const auto& v = pV->coord();
  if (u.z() == v.z()) {
    assert(u.x() == v.x() or u.y() == v.y());
    const Int_t z = u.z();
    // get the layer information
    assert(_cir.lef().bRoutingLayer(u.z()));
    const auto& layerPair = _cir.lef().layerPair(u.z());
    const auto& layer = _cir.lef().routingLayer(layerPair.second);
    const Int_t width = layer.minWidth();
    const Int_t extension = layer.minWidth() / 2;
    // generate exact wire shape
    Box<Int_t> wire;
    toWire(u, v, width, extension, wire);
    // check DRC
    if (!_drc.checkWireRoutingLayerSpacing(_net.idx(), z, wire))
      return true;
    if (!_drc.checkWireEolSpacing(_net.idx(), z, wire))
      return true;
   
    // check symmetric DRC
    if (_bSym) {
      Box<Int_t> symWire(wire);
      symWire.flipX(_cir.symAxisX());
      if (!_drc.checkWireRoutingLayerSpacing(_net.symNetIdx(), z, symWire))
        return true;
      if (!_drc.checkWireEolSpacing(_net.symNetIdx(), z, symWire))
        return true;
    }
  }
  else {
    // generate via and check min area and adj edges
    assert(u.x() == v.x() and u.y() == v.y());
    const Int_t x = u.x();
    const Int_t y = u.y();
    const Int_t botLayerIdx = std::min(u.z(), v.z());
    const LefVia& via = _cir.lef().via(botLayerIdx, 1, 1);
    if (!_drc.checkViaSpaing(_net.idx(), x, y, via))
      return true;
    // TODO: minarea, minstep
    // check min area
    //if (!checkMinArea(pU, pV)) {
      //return true;
    //}
  }
  return false;
}

bool DrGridAstar::checkMinArea(const DrGridAstarNode* pU, const DrGridAstarNode* pV) {
  const Int_t layerIdx = pU->coord().z();
  const DrGridAstarNode* pN = pU;
  Vector_t<Box<Int_t>> vPrePath;
  // get the layer information
  assert(_cir.lef().bRoutingLayer(layerIdx));
  const auto& layerPair = _cir.lef().layerPair(layerIdx);
  const auto& layer = _cir.lef().routingLayer(layerPair.second);
  const Int_t width = layer.minWidth();
  const Int_t extension = layer.minWidth() / 2;
  while (pN->pParent() and pN->pParent()->coord().z() == layerIdx) {
    // generate the exact wire shape
    Box<Int_t> wire;
    toWire(pN->coord(), pN->pParent()->coord(), width, extension, wire);
    vPrePath.emplace_back(wire);
    pN = pN->pParent();
  }
  assert(layerIdx == pN->coord().z());
  return _drc.checkWireMinArea(layerIdx, vPrePath);
}

bool DrGridAstar::bNeedUpdate(const DrGridAstarNode* pV, const Int_t costG, const Int_t bendCnt) {
  if (pV->costG() > costG)
    return true;
  else if (pV->bendCnt() > bendCnt)
    return true;
  return false;
}

void DrGridAstar::resetAllNodes() {
  for (auto& map : _vAllNodesMap) {
    for (auto& pair : map) {
      DrGridAstarNode* pNode = pair.second;
      pNode->reset();
    }
  }
}

void DrGridAstar::saveResult2Net() {
  for (const auto& vRoutedWires : _vvRoutedWires) {
    for (const auto& pair : vRoutedWires) {
      _net.vWires().emplace_back(pair);
    }
  }
  _net.setRouted(true);
  if (_bSym) {
    Net& symNet = _cir.net(_net.symNetIdx());
    for (const auto& vRoutedWires : _vvRoutedWires) {
      for (const auto& pair : vRoutedWires) {
        const Box<Int_t>& wire = pair.first;
        const Int_t layerIdx = pair.second;
        Box<Int_t> symWire(wire);
        symWire.flipX(_cir.symAxisX());
        symNet.vWires().emplace_back(symWire, layerIdx);
      }
    }
    symNet.setRouted(true);
  }
}

void DrGridAstar::ripup() {
  for (const auto& vRoutedWires : _vvRoutedWires) {
    for (const auto& pair : vRoutedWires) {
      const Box<Int_t>& wire = pair.first;
      const Int_t layerIdx = pair.second;
      bool bExist = _cir.removeSpatialRoutedWire(_net.idx(), layerIdx, wire);
      assert(bExist);
      if (_bSym) {
        Box<Int_t> symWire(wire);
        symWire.flipX(_cir.symAxisX());
        bool bExist = _cir.removeSpatialRoutedWire(_net.symNetIdx(), layerIdx, symWire);
        assert(bExist);
      }
    }
  }
}

Int_t DrGridAstar::scaledMDist(const Point3d<Int_t>& u, const Point3d<Int_t>& v) {
  Int_t dist = 0;
  dist += abs(u.x() - v.x()) * _param.horCost;
  dist += abs(u.y() - v.y()) * _param.verCost;
  dist += abs(u.z() - v.z()) * _param.viaCost;
  return dist;
}

Int_t DrGridAstar::scaledMDist(const Box<Int_t>& u, const Box<Int_t>& v) {
  Int_t dist = 0;
  dist += std::max({u.bl().x() - v.tr().x(), v.bl().x() - u.tr().x(), (Int_t)0}) * _param.horCost;
  dist += std::max({u.bl().y() - v.tr().y(), v.bl().y() - u.tr().y(), (Int_t)0}) * _param.verCost;
  return dist;
}

Int_t DrGridAstar::scaledMDist(const Point3d<Int_t>& u, const Pair_t<Box<Int_t>, Int_t>& pair) {
  Int_t dx = 0, dy = 0, dz = 0;
  const Box<Int_t>& box = pair.first;
  if (u.x() < box.xl())
    dx = box.xl() - u.x();
  else if (u.x() > box.xh())
    dx = u.x() - box.xh();
  if (u.y() < box.yl())
    dy = box.yl() - u.y();
  else if (u.y() > box.yh())
    dy = u.y() - box.yh();
  dz = abs(u.z() - pair.second);
  return dx * _param.horCost + 
         dy * _param.verCost +
         dz * _param.viaCost;
}

Int_t DrGridAstar::scaledMDist(const Pair_t<Box<Int_t>, Int_t>& u, const Pair_t<Box<Int_t>, Int_t>& v) {
  Int_t dist = scaledMDist(u.first, v.first);
  dist += abs(u.second - v.second) * _param.viaCost;
  return dist;
}

bool DrGridAstar::hasBend(const DrGridAstarNode* pU, const DrGridAstarNode* pV) {
  if (pU->pParent() != nullptr) {
    return findDir(pU->pParent()->coord(), pU->coord()) != findDir(pU->coord(), pV->coord());
  }
  return false;
}

DrGridAstar::PathDir DrGridAstar::findDir(const Point3d<Int_t>& u, const Point3d<Int_t>& v) {
  if (u.z() == v.z()) {
    if (u.x() == v.x()) {
      assert(u.y() != v.y());
      return u.y() < v.y() ? PathDir::UP : PathDir::DOWN;
    }
    else {
      assert(u.x() != v.x());
      return u.x() < v.x() ? PathDir::RIGHT : PathDir::LEFT;
    }
  }
  else {
    assert(u.x() == v.x() and u.y() == v.y());
    return u.z() < v.z() ? PathDir::VIA_UP : PathDir::VIA_DOWN;
  }
}

bool DrGridAstar::bInsideGuide(const DrGridAstarNode* pU) {
  const auto& u = pU->coord();
  assert(u.z() < (Int_t)_cir.vSpatialNetGuides(_net.idx()).size() and u.z() > 0);
  const Point<Int_t> u2d(u.x(), u.y());
  return _cir.vSpatialNetGuides(_net.idx())[u.z()].find(u2d, u2d);
}

Int_t DrGridAstar::history(const DrGridAstarNode* pU) {
  const auto& u = pU->coord();
  const Point<Int_t> u2d(u.x(), u.y());
  Vector_t<Int_t> vHis;
  _dr._vSpatialHistoryMaps[u.z()].query(u2d, u2d, vHis);
  return std::accumulate(vHis.begin(), vHis.end(), 0);
}

void DrGridAstar::toWire(const Point3d<Int_t>& u, const Point3d<Int_t>& v, const Int_t width, const Int_t extension, Box<Int_t>& wire) {
  // change wire width here
  assert(u.z() == v.z());
  const Int_t halfWidth = width / 2;
  Int_t xl = 0, yl = 0, xh = 0, yh = 0;
  if (u.x() == v.x()) { // vertical edge
    xl = u.x() - halfWidth;
    yl = std::min(u.y(), v.y()) - extension;
    xh = u.x() + halfWidth;
    yh = std::max(u.y(), v.y()) + extension;
  }
  else if (u.y() == v.y()) { // horizontal edge
    xl = std::min(u.x(), v.x()) - extension;
    yl = u.y() - halfWidth;
    xh = std::max(u.x(), v.x()) + extension;
    yh = u.y() + halfWidth;
  }
  wire.setXL(xl);
  wire.setXH(xh);
  wire.setYL(yl);
  wire.setYH(yh);
}

AcsPt::DirType DrGridAstar::findAcsDir(const DrGridAstarNode* pU) {
  assert(pU->pParent() != nullptr);
  switch (findDir(pU->coord(), pU->pParent()->coord())) {
    case PathDir::LEFT: return AcsPt::DirType::WEST;
    case PathDir::RIGHT: return AcsPt::DirType::EAST;
    case PathDir::UP: return AcsPt::DirType::NORTH;
    case PathDir::DOWN: return AcsPt::DirType::SOUTH;
    case PathDir::VIA_UP: return AcsPt::DirType::UP;
    case PathDir::VIA_DOWN: return AcsPt::DirType::DOWN;
    default: assert(false);
  }
}

void DrGridAstar::connect2AcsPt(const DrGridAstarNode* pU) {
  _vvRoutedWires.resize(_vvRoutedWires.size() + 1);
  const auto& pt = pU->coord();
  const auto& acsPt = _pinAcsMap.at(pt).gridPt();

  assert(_cir.lef().bRoutingLayer(pt.z()));
  const auto& layerPair = _cir.lef().layerPair(pt.z());
  const auto& layer = _cir.lef().routingLayer(layerPair.second);
  const Int_t width = layer.minWidth();
  const Int_t extension = width / 2;

  Box<Int_t> wire;
  toWire(pt, acsPt, width, extension, wire);
  _vvRoutedWires.back().emplace_back(wire, pt.z());
  _cir.addSpatialRoutedWire(_net.idx(), pt.z(), wire);
  if (_bSym) {
    Box<Int_t> symWire(wire);
    symWire.flipX(_cir.symAxisX());
    _cir.addSpatialRoutedWire(_net.symNetIdx(), pt.z(), symWire);
  }
}

PROJECT_NAMESPACE_END

