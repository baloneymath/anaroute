/**
 * @file   drMgr.cpp
 * @brief  Detailed Routing - Manager
 * @author Hao Chen
 * @date   09/30/2019
 *
 **/

#include "drMgr.hpp"
#include "drGridlessRoute.hpp"

PROJECT_NAMESPACE_START

void DrMgr::solve() {
  
  runGridlessRoute();
}


void DrMgr::runGridlessRoute() {
  DrGridlessRoute kernel(*this, _cir);
  kernel.solve();
}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////

PROJECT_NAMESPACE_END
