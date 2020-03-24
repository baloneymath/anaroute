/**
 * @file   drGridRoute.cpp
 * @brief  Detailed Routing - Grid Based Routing
 * @author Hao Chen
 * @date   02/01/2020
 *
 **/

#include "drGridRoute.hpp"
#include "drGridAstar.hpp"
#include "drRoutable.hpp"

PROJECT_NAMESPACE_START

void DrGridRoute::solve() {
  
  // initialize net routing priority queue
  PairingHeap<Net*, Net_Cmp> pq;

  // add unrouted nets to pq
  addUnroutedNetsToPQ(pq);

  // start routing process with ripup and reroute
  if (runNRR(pq, _param.maxIteration)) {
    fprintf(stderr, "DrGridRoute::%s Solved!!!!\n", __func__);
    return;
  }
  
  // second try
  fprintf(stderr, "DrGridRoute::%s Second Stage\n", __func__);
  // reset history map
  for (auto& historyMap : _vSpatialHistoryMaps) {
    historyMap.clear();
  }
  // reset net fail cnt
  for (Int_t i = 0; i < (Int_t)_cir.numNets(); ++i) {
    _cir.net(i).clearDrFail();
  }
  if (runNRR(pq, _param.maxIteration2)) {
    fprintf(stderr, "DrGridRoute::%s Solved!!!!\n", __func__);
    return;
  }
  
  // Failed QQ
  fprintf(stderr, "DrGridRoute::%s Failed!!!!\n", __func__);
  fprintf(stderr, "Unrouted Nets:");
  for (Int_t i = 0; i < (Int_t)_cir.numNets(); ++i) {
    const Net& net = _cir.net(i);
    if (!net.bRouted()) {
      fprintf(stderr, " %s", net.name().c_str());
    }
  }
  fprintf(stderr, "\n");
}

bool DrGridRoute::runNRR(PairingHeap<Net*, Net_Cmp>& pq, const Int_t maxIteration) {
  for (Int_t iter = 0; iter < maxIteration; ++iter) {
    fprintf(stderr, "\nDrGridRoute::%s\tIteration %d Unrouted nets %d\n", __func__, iter, (Int_t)pq.size());
    while (!pq.empty()) {
      Net* pNet = pq.top();
      pq.pop();

      // start Astar routing (Hard DRC)
      bool bSuccess = routeSingleNet(*pNet, true);
      if (!bSuccess) {
        bSuccess = routeSingleNet(*pNet, false);
      }
      assert(bSuccess);
    }
    // check DRC violations
    bool bFinish = checkDRC();
    if (bFinish) {
      return true;
    }
    else {
      addUnroutedNetsToPQ(pq);
    }
  }
  return false;
}

void DrGridRoute::addUnroutedNetsToPQ(PairingHeap<Net*, Net_Cmp>& pq) {
  for (Int_t i = 0; i < (Int_t)_cir.numNets(); ++i) {
    Net* pNet = &_cir.net(i);
    if (pNet->numPins() > 1) {
      if (!pNet->bRouted())
        //if (pNet->bPower())
        //if (pNet->name() == "VDD")
        pq.push(pNet);
    }
    else { // single pin net
      pNet->setRouted(true);
    }
  }
}

void DrGridRoute::checkSymSelfSym(const Net& net, const Routable& ro, bool& bSym, bool& bSelfSym) {
  bSym = ro.hasSymNet();
  bSelfSym = ro.bSelfSym();
  if (net.hasSymNet()) {
    if (net.drFailCnt() >= _param.maxSymTry
        or _cir.net(net.symNetIdx()).drFailCnt() >= _param.maxSymTry) {
      bSym = false;
    }
  }
  if (net.bSelfSym()) {
    if (net.drFailCnt() >= _param.maxSelfSymTry) {
      bSelfSym = false;
    }
  }
}

bool DrGridRoute::routeSingleNet(Net& net, const bool bStrictDRC) {
  
  if (net.hasSymNet()) {
    assert(!net.bPower());
    Net& symNet = _cir.net(net.symNetIdx());
    if (net.drFailCnt() >= _param.maxSymTry
        or symNet.drFailCnt() >= _param.maxSymTry) {
      net.clearRoutables();
      symNet.clearRoutables();
      DrRoutable drRo(_cir);
      drRo.constructNetRoutables(net, false, false);
    }
  }
  if (net.bSelfSym()) {
    if (net.drFailCnt() >= _param.maxSelfSymTry) {
      net.clearRoutables();
      DrRoutable drRo(_cir);
      drRo.constructNetRoutables(net, false, false);
    }
  }
  
  for (Int_t i = 0; i < net.numRoutables(); ++i) {
    auto& ro = net.routable(i);
    // check sym and self sym
    bool bSym = false;
    bool bSelfSym = false;
    checkSymSelfSym(net, ro, bSym, bSelfSym);


    DrGridAstar kernel(_cir, net, ro, this->_drc, *this, bSym, bSelfSym, bStrictDRC);
    if (!kernel.run())
      return false;
  }
  return true;
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
      //for (Int_t j = 0; j < (Int_t)_cir.numNets(); ++j) {
        //ripupSingleNet(_cir.net(j));
      //}
      bValid = false;
    }
  }
  return bValid;
}

bool DrGridRoute::checkSingleNetDRC(const Net& net) {
  // check same net
  if (!_drc.checkSameNetRoutingLayerSpacing(net.idx()))
    return false;
  // check other nets
  for (const auto& pair : net.vWires()) {
    const auto& wire = pair.first;
    const Int_t layerIdx = pair.second;
    if (_cir.lef().bRoutingLayer(layerIdx)) {
      const Int_t prl = std::max(wire.width(), wire.height());
      if (!_drc.checkWireRoutingLayerSpacing(net.idx(), layerIdx, wire, prl))
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
  }
  net.addDrFail();
  net.clearRouting();
  fprintf(stderr, "DrGridRoute::%s Ripup net %s (fail %d)\n", __func__, net.name().c_str(), net.drFailCnt());
  // check sym net
  if (net.hasSymNet()) {
    Net& symNet = _cir.net(net.symNetIdx());
    for (const auto& pair : symNet.vWires()) {
      const auto& wire = pair.first;
      const Int_t layerIdx = pair.second;
      bool bExist = _cir.removeSpatialRoutedWire(symNet.idx(), layerIdx, wire);
      assert(bExist);
    }
    symNet.addDrFail();
    symNet.clearRouting();
    fprintf(stderr, "DrGridRoute::%s Ripup net %s (fail %d)\n", __func__, symNet.name().c_str(), symNet.drFailCnt());
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

PROJECT_NAMESPACE_END

