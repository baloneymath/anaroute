/**
 * @file   drAstar.cpp
 * @brief  Detailed Routing - A* search kernel
 * @author Hao Chen
 * @date   10/25/2019
 *
 **/

#include "drAstar.hpp"

#include <lemon/maps.h>
#include <lemon/kruskal.h>
#include <lemon/list_graph.h>

PROJECT_NAMESPACE_START

bool DrAstar::runKernel() {
  if (_net.bSelfSym()) {
    computeSelfSymAxisX();
    if (!bSatisfySelfSymCondition()) {
      fprintf(stderr, "DrAstar::%s WARNING: Net %s does not satisfy self symmetric condition!\n", __func__, _net.name().c_str());
    }
  }
  else if (_net.hasSymNet()) {
    computeSymAxisX();
    if (!bSatisfySymCondition()) {
      fprintf(stderr, "DrAstar::%s WARNING: Net %s does not satisfy symmetric net condition!\n", __func__, _net.name().c_str());
    }
  }
  
  initAcsPoints();
  splitSubNetMST();

  return true;
}

void DrAstar::initAcsPoints() {
  _compDS.init(_net.numPins());
  _vCompBoxes.resize(_net.numPins());
  _vCompAcsPts.resize(_net.numPins());
  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  Net_ForEachPinIdx(_net, pinIdx, i) {
    _vCompAcsPts[i].set_empty_key(Point3d<Int_t>(MIN_INT, MIN_INT, MIN_INT));
    _vCompAcsPts[i].set_deleted_key(Point3d<Int_t>(MAX_INT, MAX_INT, MAX_INT));
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      const Pair_t<LefLayerType, UInt_t>& layerPair = _cir.lef().layerPair(layerIdx);
      assert(layerPair.first == LefLayerType::ROUTING);
      const LefRoutingLayer& layer = _cir.lef().routingLayer(layerPair.second);
      const Int_t minWidth = layer.minWidth();
      const Int_t halfWidth = minWidth / 2;
      Pin_ForEachLayerBoxC(pin, layerIdx, cpBox, j) {
        _vCompBoxes[i].emplace_back((*cpBox), layerIdx);
        const Int_t minX = cpBox->min_corner().x();  
        const Int_t minY = cpBox->min_corner().y();
        const Int_t maxX = cpBox->max_corner().x();
        const Int_t maxY = cpBox->max_corner().y();
        // make peripheral acs points
        for (Int_t x = minX + halfWidth, y = minY; x + halfWidth <= maxX; x += minWidth) {
          Point3d<Int_t> acsPt(x, y, layerIdx);
          _vCompAcsPts[i].insert(acsPt);
        }
        for (Int_t x = maxX, y = minY + halfWidth; y + halfWidth <= maxY; y += minWidth) {
          Point3d<Int_t> acsPt(x, y, layerIdx);
          _vCompAcsPts[i].insert(acsPt);
        }
        for (Int_t x = maxX - halfWidth, y = maxY; x - halfWidth >= minX; x -= minWidth) {
          Point3d<Int_t> acsPt(x, y, layerIdx);
          _vCompAcsPts[i].insert(acsPt);
        }
        for (Int_t x = minX, y = maxY - halfWidth; y - halfWidth >= minY; y -= minWidth) {
          Point3d<Int_t> acsPt(x, y, layerIdx);
          _vCompAcsPts[i].insert(acsPt);
        }
      }
    }
  }

}

void DrAstar::splitSubNetMST() {
  lemon::ListGraph graph;
  Vector_t<lemon::ListGraph::Node> vGraphNodes;
  Vector_t<lemon::ListGraph::Edge> vGraphEdges;
  lemon::ListGraph::EdgeMap<Int_t> edgeCostMap(graph);

  Vector_t<Int_t> vEdgeCostValues;
  UInt_t i, j;
  for (i = 0; i < _vCompBoxes.size(); ++i) {
    vGraphNodes.emplace_back(graph.addNode());
  }
  for (i = 0; i < _vCompBoxes.size(); ++i) {
    for (j = i + 1; j < _vCompBoxes.size(); ++j) {
      vGraphEdges.emplace_back(graph.addEdge(vGraphNodes[i], vGraphNodes[j]));
      Int_t minDist = MAX_INT;
      for (const Pair_t<Box<Int_t>, Int_t>& u : _vCompBoxes[i]) {
        for (const Pair_t<Box<Int_t>, Int_t>& v : _vCompBoxes[j]) {
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

  Vector_t<lemon::ListGraph::Edge> vResEdges;
  lemon::kruskal(graph, edgeCostMap, std::back_inserter(vResEdges));
  assert(vResEdges.size() == _vCompBoxes.size() - 1);
  for (i = 0; i < vResEdges.size(); ++i) {
    lemon::ListGraph::Edge& edge = vResEdges[i];
    _vCompPairs.emplace_back(graph.id(graph.u(edge)), graph.id(graph.v(edge)));
  }
}


bool DrAstar::routeSubNet(UInt_t srcIdx, UInt_t tarIdx) {

}

bool DrAstar::computeSelfSymAxisX() {
  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  Int_t numBoxes = 0;
  Net_ForEachPinIdx(_net, pinIdx, i) {
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      numBoxes += pin.numBoxes(layerIdx);
      Pin_ForEachLayerBoxC(pin, layerIdx, cpBox, j) {
        _selfSymAxisX += cpBox->center().x();
      }
    }
  }
  _selfSymAxisX /= numBoxes;
}


bool DrAstar::computeSymAxisX() {
  Net& symNet = _cir.net(_net.symNetIdx());
  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  Int_t numBoxes = 0;
  Net_ForEachPinIdx(_net, pinIdx, i) {
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      numBoxes += pin.numBoxes(layerIdx);
      Pin_ForEachLayerBoxC(pin, layerIdx, cpBox, j) {
        _symAxisX += cpBox->center().x();
      }
    }
  }
  Net_ForEachPinIdx(symNet, pinIdx, i) {
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      numBoxes += pin.numBoxes(layerIdx);
      Pin_ForEachLayerBoxC(pin, layerIdx, cpBox, j) {
        _symAxisX += cpBox->center().x();
      }
    }
  }
  _symAxisX /= numBoxes;
}

bool DrAstar::bSatisfySelfSymCondition() const {
  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  Net_ForEachPinIdx(_net, pinIdx, i) {
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBoxC(pin, layerIdx, cpBox, j) {
        
      }
    }
  }
  return true;
}

bool DrAstar::bSatisfySymCondition() const {
  return true;
}
/////////////////////////////////////////
//    Helper functions                 //
/////////////////////////////////////////
Int_t DrAstar::scaledMDist(const Point3d<Int_t>& u, const Point3d<Int_t>& v) {
  Int_t dist = 0;
  dist += abs(u.x() - v.x()) * _param.horCost;
  dist += abs(u.y() - v.y()) * _param.verCost;
  dist += abs(u.z() - v.z()) * _param.viaCost;
  return dist;
}

Int_t DrAstar::scaledMDist(const Box<Int_t>& u, const Box<Int_t>& v) {
  Int_t dist = 0;
  dist += std::max({u.bl().x() - v.tr().x(), v.bl().x() - u.tr().x(), 0}) * _param.horCost;
  dist += std::max({u.bl().y() - v.tr().y(), v.bl().y() - u.tr().y(), 0}) * _param.verCost;
  return dist;
}

Int_t DrAstar::scaledMDist(const Pair_t<Box<Int_t>, Int_t>& u, const Pair_t<Box<Int_t>, Int_t>& v) {
  Int_t dist = scaledMDist(u.first, v.first);
  dist += abs(u.second - v.second) * _param.viaCost;
  return dist;
}

PROJECT_NAMESPACE_END

