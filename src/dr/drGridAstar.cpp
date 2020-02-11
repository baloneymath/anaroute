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

#include <lemon/maps.h>
#include <lemon/kruskal.h>
#include <lemon/list_graph.h>

PROJECT_NAMESPACE_START

bool DrGridAstar::run() {
  
  if (_net.bRouted())
    return true;

  init();
  splitSubNetMST();

  bool bSym = false;
  bool bSelfSym = false;

  // check sym and self-sym constraint
  if (_net.hasSymNet()
      and _net.drFailCnt() < _param.maxSymTry
      and _cir.net(_net.symNetIdx()).drFailCnt() < _param.maxSymTry) {
    if (bSatisfySymCondition()) {
      bSym = true;
    }
    else {
      fprintf(stderr, "DrGridAstar::%s WARNING: Net %s %s cannot be symmetric\n", __func__, _net.name().c_str(), _cir.net(_net.symNetIdx()).name().c_str());
    }
  }
  else if (_net.bSelfSym()
           and _net.drFailCnt() < _param.maxSelfSymTry) {
    if (bSatisfySelfSymCondition()) {
      bSelfSym = true;
    }
    else {
      fprintf(stderr, "DrGridAstar::%s WARNING: Net %s cannot be self-symmetric\n", __func__, _net.name().c_str());
    }
  }

  // route net with sym and self-sym constraint
  bool bSuccess = route(bSym, bSelfSym);

  // ripup or not
  if (!bSuccess) {
    ripup();
    return false;
  }
  saveResult2Net(bSym);
  return true;
}

void DrGridAstar::init() {
  _compDS.init(_net.numPins());
  _vCompBoxes.resize(_net.numPins());
  _vCompAcsPts.resize(_net.numPins());
  _vCompSpatialBoxes.resize(_net.numPins());

  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  const Point3d<Int_t>* cpPt;
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
      Point3d<Int_t> acs(_cir.gridOffsetX() + cpPt->x() * _cir.gridStep(),
                         _cir.gridOffsetY() + cpPt->y() * _cir.gridStep(),
                         cpPt->z());
      _vCompAcsPts[i].insert(acs);
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

bool DrGridAstar::bSatisfySymCondition() {
  const Int_t symAxisX = _cir.symAxisX();
  const Net& net1 = _net;
  const Net& net2 = _cir.net(_net.symNetIdx());
  Vector_t<Box<Int_t>> vBoxes1, vBoxes2;
  // init net1 pin shapes
  for (const auto idx : net1.vPinIndices()) {
    const Pin& pin = _cir.pin(idx);
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vBoxes1.emplace_back(*cpBox);
      }
    }
  }
  // init net2 pin shapes
  for (const auto idx : net2.vPinIndices()) {
    const Pin& pin = _cir.pin(idx);
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vBoxes2.emplace_back(*cpBox);
      }
    }
  }
  // search net1 sym shapes in net2
  if (vBoxes1.size() != vBoxes2.size())
    return false;
  std::sort(vBoxes2.begin(), vBoxes2.end());
  for (const auto& box : vBoxes1) {
    Box<Int_t> symBox;
    symBox.setXL(2 * symAxisX - box.xh());
    symBox.setYL(box.yl());
    symBox.setXH(2 * symAxisX - box.xl());
    symBox.setYH(box.yh());
    if (!std::binary_search(vBoxes2.begin(), vBoxes2.end(), symBox)) {
      return false;
    }
  }
  return true;
}

bool DrGridAstar::bSatisfySelfSymCondition() {
  const Int_t symAxisX = _cir.symAxisX();
  Vector_t<Box<Int_t>> vBoxes;
  // init pin shapes
  for (const auto idx : _net.vPinIndices()) {
    const Pin& pin = _cir.pin(idx);
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vBoxes.emplace_back(*cpBox);
      }
    }
  }
  // make sure there exist a sym box of each box
  std::sort(vBoxes.begin(), vBoxes.end());
  for (const auto& box : vBoxes) {
    Box<Int_t> symBox;
    symBox.setXL(2 * symAxisX - box.xh());
    symBox.setYL(box.yl());
    symBox.setXH(2 * symAxisX - box.xl());
    symBox.setYH(box.yh());
    if (!std::binary_search(vBoxes.begin(), vBoxes.end(), symBox)) {
      return false;
    }
  }
  return true;
}

bool DrGridAstar::route(const bool bSym, const bool bSelfSym) {
  for (const auto& pair : _vSubNets) {
    const Int_t srcIdx = pair.first;
    const Int_t tarIdx = pair.second;
    if (!routeSubNet(srcIdx, tarIdx, bSym, bSelfSym)) {
      fprintf(stderr, "DrGridAstar::%s ERROR: Route net %s failed!\n", __func__, _net.name().c_str());
      return false;
    }
  }
  return true;
}

bool DrGridAstar::routeSubNet(Int_t srcIdx, Int_t tarIdx, const bool bSym, const bool bSelfSym) {
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

  return pathSearch(srcIdx, tarIdx, bSym, bSelfSym);
}

 bool DrGridAstar::pathSearch(const Int_t srcIdx, const Int_t tarIdx, const bool bSym, const bool bSelfSym) {
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
    if (tar.find(pU->coord()) != tar.end()) { // u \in tar
      const UInt_t bigCompIdx = mergeComp(srcIdx, tarIdx);
      backTrack(pU, bigCompIdx, srcIdx, tarIdx, bSym);
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
      if (bViolateDRC(pU, pV, bSym, bSelfSym)) {
        continue;
      }
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
        if (bInsideGuide(pV)) {
          costF += _param.guideCost;
        }
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

void DrGridAstar::backTrack(const DrGridAstarNode* pU, const Int_t bigCompIdx, const Int_t srcIdx, const Int_t tarIdx, const bool bSym) {
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
  
  savePath(lPathVec, bSym);
  

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
      Box<Int_t> wire;
      toWire(u, v, wire);
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

void DrGridAstar::savePath(const List_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>& lPathVec, const bool bSym) {
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
      Box<Int_t> wire;
      toWire(u, v, wire);
      vRoutedWires.emplace_back(wire, u.z());
      _cir.addSpatialRoutedWire(_net.idx(), u.z(), wire);
      // add symmetric wire to spatial routed wire, for DRC
      if (bSym) {
        Box<Int_t> symWire(wire);
        symWire.flipX(_cir.symAxisX());
        _cir.addSpatialRoutedWire(_net.symNetIdx(), u.z(), symWire);
      }
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
      if (bSym) {
        const Int_t symX = 2 * _cir.symAxisX() - x;
        _cir.addSpatialRoutedVia(_net.idx(), symX, y, via);
      }
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

bool DrGridAstar::bViolateDRC(const DrGridAstarNode* pU, const DrGridAstarNode* pV, const bool bSym, const bool bSelfSym) {
  const auto& u = pU->coord();
  const auto& v = pV->coord();
  if (u.z() == v.z()) {
    assert(u.x() == v.x() or u.y() == v.y());
    const Int_t z = u.z();
    Box<Int_t> wire;
    toWire(u, v, wire);
    // check DRC
    if (!_drc.checkWireRoutingLayerSpacing(_net.idx(), z, wire))
      return true;
    if (!_drc.checkWireEolSpacing(_net.idx(), z, wire))
      return true;
   
    // check symmetric DRC
    if (bSym) {
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
    //const DrGridAstarNode* pN = pU;
    //Vector_t<Box<Int_t>> vPrePath;
    //Int_t numBends = 0;
    //Int_t totalArea = 0;
    //while (pN->pParent() and pN->pParent()->coord().z() == u.z()) {
      //Box<Int_t> wire;
      //toWire(u, pN->pParent()->coord(), wire);
      //totalArea += wire.area();
      //if (hasBend(pN->pParent(), pN)) {
        //numBends += 1;
      //}
      //pN = pN->pParent();
    //}
    //totalArea -= 
  }
  return false;
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

void DrGridAstar::ripup() {
  for (const auto& vRoutedWires : _vvRoutedWires) {
    for (const auto& pair : vRoutedWires) {
      const Box<Int_t>& wire = pair.first;
      const Int_t layerIdx = pair.second;
      _cir.removeSpatialRoutedWire(_net.idx(), layerIdx, wire);
    }
  }
}

void DrGridAstar::saveResult2Net(const bool bSym) {
  for (const auto& vRoutedWires : _vvRoutedWires) {
    for (const auto& pair : vRoutedWires) {
      _net.vWires().emplace_back(pair);
    }
  }
  if (bSym) {
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
  const Point3d<Int_t>& u = pU->coord();
  assert(u.z() < (Int_t)_cir.vSpatialNetGuides(_net.idx()).size() and u.z() > 0);
  const Point<Int_t> u2d(u.x(), u.y());
  return _cir.vSpatialNetGuides(_net.idx())[u.z()].find(u2d, u2d);
}

void DrGridAstar::toWire(const Point3d<Int_t>& u, const Point3d<Int_t>& v, Box<Int_t>& wire) {
  // change wire width here
  assert(u.z() == v.z());
  const Int_t z = u.z();
  const Pair_t<LefLayerType, UInt_t>& layerPair = _cir.lef().layerPair(z);
  assert(layerPair.first == LefLayerType::ROUTING);
  const LefRoutingLayer& layer = _cir.lef().routingLayer(layerPair.second);
  const Int_t halfWidth = layer.minWidth() / 2;
  const Int_t xl = std::min(u.x(), v.x()) - halfWidth;
  const Int_t xh = std::max(u.x(), v.x()) + halfWidth;
  const Int_t yl = std::min(u.y(), v.y()) - halfWidth;
  const Int_t yh = std::max(u.y(), v.y()) + halfWidth;
  wire.setXL(xl);
  wire.setXH(xh);
  wire.setYL(yl);
  wire.setYH(yh);
}

PROJECT_NAMESPACE_END

