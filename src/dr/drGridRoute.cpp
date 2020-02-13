/**
 * @file   drGridRoute.cpp
 * @brief  Detailed Routing - Grid Based Routing
 * @author Hao Chen
 * @date   02/01/2020
 *
 **/

#include "drGridRoute.hpp"
#include "drGridAstar.hpp"
#include "src/ds/pqueue.hpp"

PROJECT_NAMESPACE_START

void DrGridRoute::solve() {
  
  // initialize net routing priority queue
  PairingHeap<Net*, Net_Cmp> pq;
  UInt_t i;
  Net* pNet;
  Cir_ForEachNet(_cir, pNet, i) {
    if (pNet->numPins() > 1) {
      pq.push(pNet);
    }
  }

  // start routing process with ripup and reroute
  while (!pq.empty()) {
    pNet = pq.top();
    pq.pop();
    
    // check sym and self sym
    bool bSym = false;
    bool bSelfSym = false;
    checkSymSelfSym(*pNet, bSym, bSelfSym);

    // start Astar routing (soft DRC)
    bool bSuccess = routeSingleNet(*pNet, bSym, bSelfSym, false);
    assert(bSuccess);
  }
  // check DRC violations
  //checkDRC();

}

void DrGridRoute::checkSymSelfSym(const Net& net, bool& bSym, bool& bSelfSym) {
  if (net.hasSymNet()
      and net.drFailCnt() < _param.maxSymTry
      and _cir.net(net.symNetIdx()).drFailCnt() < _param.maxSymTry) {
    if (bSatisfySymCondition(net)) {
      bSym = true;
    } 
    else {
      fprintf(stderr, "DrGridRoute::%s WARNING: Net %s %s cannot be symmetric\n", __func__, net.name().c_str(), _cir.net(net.symNetIdx()).name().c_str());
    }
  }
  else if (net.bSelfSym()
           and net.drFailCnt() < _param.maxSelfSymTry) {
    if (bSatisfySelfSymCondition(net)) {
      bSelfSym = true;
    }
    else {
      fprintf(stderr, "DrGridRoute::%s WARNING: Net %s cannot be self-symmetric\n", __func__, net.name().c_str());
    }
  }

}

bool DrGridRoute::routeSingleNet(Net& n, const bool bSym, const bool bSelfSym, const bool bStrictDRC) {
  DrGridAstar kernel(_cir, n, this->_drc, *this, bSym, bSelfSym, bStrictDRC);
  return kernel.run();
}

bool DrGridRoute::checkDRC() {
  bool bValid = true;
  for (Int_t i = 0; i < (Int_t)_cir.numNets(); ++i) {
    Net& net = _cir.net(i);
    if (!checkSingleNetDRC(net)) {
      ripupSingleNet(net);
      bValid = false;
    }
  }
  return bValid;
}

bool DrGridRoute::checkSingleNetDRC(const Net& net) {
  for (const auto& pair : net.vWires()) {
    const auto& wire = pair.first;
    const Int_t layerIdx = pair.second;
    if (_cir.lef().bRoutingLayer(layerIdx)) {
      if (!_drc.checkWireRoutingLayerSpacing(net.idx(), layerIdx, wire))
        return false;
      if (!_drc.checkWireEolSpacing(net.idx(), layerIdx, wire))
        return false;
    }
    else {
      assert(_cir.lef().bCutLayer(layerIdx));
      if (!_drc.checkWireCutLayerSpacing(net.idx(), layerIdx, wire))
        return false;
    }
  }
  return true;
}

void DrGridRoute::ripupSingleNet(Net& n) {
  // TODO
}


void DrGridRoute::addWireHistoryCost(const Int_t cost, const Int_t layerIdx, const Box<Int_t>& wire) {
  _vSpatialHistoryMaps[layerIdx].insert(wire, cost);
}

void DrGridRoute::addViaHistoryCost(const Int_t cost, const Int_t x, const Int_t y, const LefVia& via) {
  for (auto box : via.vBotBoxes()) {
    box.shift(x, y);
    addWireHistoryCost(cost, via.botLayerIdx(), box);
  }
  for (auto box : via.vCutBoxes()) {
    box.shift(x, y);
    addWireHistoryCost(cost, via.cutLayerIdx(), box);
  }
  for (auto box : via.vTopBoxes()) {
    box.shift(x, y);
    addWireHistoryCost(cost, via.topLayerIdx(), box);
  }
}

bool DrGridRoute::bSatisfySymCondition(const Net& net) {
  const Int_t symAxisX = _cir.symAxisX();
  const Net& symNet = _cir.net(net.symNetIdx());
  Vector_t<Box<Int_t>> vBoxes1, vBoxes2;
  // init net1 pin shapes
  for (const auto idx : net.vPinIndices()) {
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
  for (const auto idx : symNet.vPinIndices()) {
    const Pin& pin = _cir.pin(idx);
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vBoxes2.emplace_back(*cpBox);
      }
    }
  }
  // search net sym shapes in symNet
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

bool DrGridRoute::bSatisfySelfSymCondition(const Net& net) {
  const Int_t symAxisX = _cir.symAxisX();
  Vector_t<Box<Int_t>> vBoxes;
  // init pin shapes
  for (const auto idx : net.vPinIndices()) {
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

PROJECT_NAMESPACE_END

