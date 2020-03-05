/**
 * @file   drMgr.cpp
 * @brief  Detailed Routing - Manager
 * @author Hao Chen
 * @date   09/30/2019
 *
 **/

#include "drMgr.hpp"
#include "drGridlessRoute.hpp"
#include "drGridRoute.hpp"
#include "drSymmetry.hpp"
#include "drRoutable.hpp"
#include "drPower.hpp"

PROJECT_NAMESPACE_START

void DrMgr::solve(const bool bGrid, const bool bUseSymFile) {
  fprintf(stderr, "\nDrMgr::%s Start Detailed Routing\n", __func__);
  if (bGrid)
    runGridRoute(bUseSymFile);
  else
    runGridlessRoute();
}

void DrMgr::runGridlessRoute() {
  DrGridlessRoute kernel(_cir, *this, _drcMgr);
  kernel.solve();
}

void DrMgr::runGridRoute(const bool bUseSymFile) {

  DrSymmetry sym(_cir);
  sym.solve(bUseSymFile);

  DrRoutable ro(_cir);
  ro.constructRoutables();

  DrGridRoute kernel(_cir, *this, _drcMgr);
  kernel.solve();
}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////

PROJECT_NAMESPACE_END
