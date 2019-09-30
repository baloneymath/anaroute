/**
 * @file   drMgr.cpp
 * @brief  Detailed Routing - Manager
 * @author Hao Chen
 * @date   09/30/2019
 *
 **/

#include "drMgr.hpp"
#include "src/ds/pqueue.hpp"

PROJECT_NAMESPACE_START

void DrMgr::solve() {
  
  PairingHeap<Net*, Net_Cmp> pqueue;
  
  Net* pNet;
  Index_t i;
  Cir_ForEachNet(_cir, pNet, i) {
    // ignore dangling net
    if (pNet->numPins() > 1) {
      pqueue.push(pNet);
    }
  }

}

PROJECT_NAMESPACE_END
