/**
 * @file   drGridRoute.cpp
 * @brief  Detailed Routing - Grid Based Routing
 * @author Hao Chen
 * @date   02/01/2020
 *
 **/

#include "drGridRoute.hpp"
#include "drGridAstar.hpp"

PROJECT_NAMESPACE_START

void DrGridRoute::solve() {
  
  // initialize net routing priority queue
  PairingHeap<Net*, Net_Cmp> pq;

  // add unrouted nets to pq
  addUnroutedNetsToPQ(pq);

  // start routing process with ripup and reroute
  for (Int_t iter = 0; iter < _param.maxIteration; ++iter) {
    fprintf(stderr, "DrGridRoute::%s Iteration %d Unrouted nets %d\n", __func__, iter, (Int_t)pq.size());
    while (!pq.empty()) {
      Net* pNet = pq.top();
      pq.pop();
      
      // check sym and self sym
      bool bSym = false;
      bool bSelfSym = false;
      checkSymSelfSym(*pNet, bSym, bSelfSym);

      // start Astar routing (Hard DRC)
      bool bSuccess = routeSingleNet(*pNet, bSym, bSelfSym, true);
      if (!bSuccess) {
        bSuccess = routeSingleNet(*pNet, bSym, bSelfSym, false);
      }
      assert(bSuccess);
    }
    // check DRC violations
    bool bFinish = checkDRC();
    if (bFinish) {
      break;
    }
    else {
      addUnroutedNetsToPQ(pq);
    }
  }
  // second try
  if (!pq.empty()) {
    fprintf(stderr, "DrGridRoute::%s Second Stage\n", __func__);
    for (auto& historyMap : _vSpatialHistoryMaps) {
      historyMap.clear();
    }
    for (Int_t iter = 0; iter < _param.maxIteration2; ++iter) {
      fprintf(stderr, "DrGridRoute::%s Second Stage Iteration %d Unrouted nets %d\n", __func__, iter, (Int_t)pq.size());
      while (!pq.empty()) {
        Net* pNet = pq.top();
        pq.pop();
        
        bool bSym = false;
        bool bSelfSym = false;
        checkSymSelfSym(*pNet, bSym, bSelfSym);
        
        bool bSuccess = routeSingleNet(*pNet, bSym, bSelfSym, true);
        if (!bSuccess) {
          bSuccess = routeSingleNet(*pNet, bSym, bSelfSym, false);
        }
        assert(bSuccess);
      }
      bool bFinish = checkDRC();
      if (bFinish) {
        break;
      }
      else {
        addUnroutedNetsToPQ(pq);
      }
    }
  }


}

void DrGridRoute::addUnroutedNetsToPQ(PairingHeap<Net*, Net_Cmp>& pq) {
  for (Int_t i = 0; i < (Int_t)_cir.numNets(); ++i) {
    Net* pNet = &_cir.net(i);
    if (!pNet->bRouted() and pNet->numPins() > 1) {
      pq.push(pNet);
    }
  }
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
  // randomize the checking sequence
  Vector_t<Int_t> vIndices(_cir.numNets(), 0);
  std::iota(vIndices.begin(), vIndices.end(), 0);
  std::random_shuffle(vIndices.begin(), vIndices.end());
  bool bValid = true;
  for (auto i : vIndices) {
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

void DrGridRoute::ripupSingleNet(Net& net) {
  for (const auto& pair : net.vWires()) {
    const auto& wire = pair.first;
    const Int_t layerIdx = pair.second;
    bool bExist = _cir.removeSpatialRoutedWire(net.idx(), layerIdx, wire);
    assert(bExist);
    net.setRouted(false);
    // check sym net
    if (net.hasSymNet()) {
      Net& symNet = _cir.net(net.symNetIdx());
      if (symNet.bRouted()) {
        for (const auto& pair : symNet.vWires()) {
          const auto& wire = pair.first;
          const Int_t layerIdx = pair.second;
          bool bExist = _cir.removeSpatialRoutedWire(symNet.idx(), layerIdx, wire);
          assert(bExist);
          symNet.setRouted(false);
        }
      }
    }
  }
  net.vWires().clear();
  if (net.hasSymNet()) {
    Net& symNet = _cir.net(net.symNetIdx());
    symNet.vWires().clear();
  }
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

