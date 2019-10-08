/**
 * @file   grMgr.cpp
 * @brief  Global Routing - Manager
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/


#include "grMgr.hpp"
#include "grGridRoute.hpp"

PROJECT_NAMESPACE_START

void GrMgr::solve() {
  runGridRoute();
}

void GrMgr::runGridRoute() {
  GrGridRoute kernel(*this, _cir);
  kernel.solve();
}

PROJECT_NAMESPACE_END
