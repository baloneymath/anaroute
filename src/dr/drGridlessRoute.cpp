/**
 * @file   drGridlessRoute.cpp
 * @brief  Detailed Routing - Gridless Routing
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#include "drGridlessRoute.hpp"
#include "drAstar.hpp"
#include "src/ds/pqueue.hpp"

PROJECT_NAMESPACE_START

void DrGridlessRoute::solve() {
  
  //_vAllAstarNodesMap.resize(_cir.lef().numLayers());
  //for (auto& m : _vAllAstarNodesMap)
    //m.set_empty_key(Point<Int_t>(-1, -1));

  PairingHeap<Net*, Net_Cmp> pq;
  UInt_t i;
  Net* pNet;
  Cir_ForEachNet(_cir, pNet, i) {
    // ignore dangling nets
    if (pNet->numPins() > 1) {
      //if (pNet->hasSymNet() and pNet->idx() > pNet->symNetIdx()) {
        //continue;
      //}
      //if (pNet->name() == "AVSS")
      pq.push(pNet);
    }
  }
  // rip up and reroute
  Queue_t<Net*> qFrozenNet;
  while (!pq.empty() or !qFrozenNet.empty()) {
    while (qFrozenNet.size() > _rrParams.numFrozen) {
      pq.push(qFrozenNet.front());
      qFrozenNet.pop();
    }
    if (pq.empty()) {
      pq.push(qFrozenNet.front());
      qFrozenNet.pop();
    }
    pNet = pq.top();
    std::cerr << pNet->name() << std::endl;
    pq.pop();
    if (!routeSingleNet(*pNet)) {
      pNet->addDrFail();
      qFrozenNet.push(pNet);
    }
  }
}

bool DrGridlessRoute::routeSingleNet(Net& n) {
  DrAstar astarKernel(_cir, n, *this);
  return astarKernel.runKernel();
}

PROJECT_NAMESPACE_END
