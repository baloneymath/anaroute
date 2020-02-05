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
    if (!routeSingleNet(*pNet)) {
      pNet->addDrFail();
      // TODO: ripup some nets
    }
  }

}


bool DrGridRoute::routeSingleNet(Net& n) {
  DrGridAstar kernel(_cir, n, this->_drcMgr);
  return kernel.run();
}

void DrGridRoute::ripupSingleNet(Net& n) {

}

PROJECT_NAMESPACE_END

