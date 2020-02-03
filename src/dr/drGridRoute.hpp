/**
 * @file   drGridlRoute.hpp
 * @brief  Detailed Routing - Grid Based Routing
 * @author Hao Chen
 * @date   02/01/2020
 *
 **/

#ifndef _DR_GRID_ROUTE_HPP_
#define _DR_GRID_ROUTE_HPP_

#include "drMgr.hpp"

PROJECT_NAMESPACE_START

class DrGridRoute {
 public:
  DrGridRoute(CirDB& c, DrMgr& dr, DrcMgr& drc)
    : _cir(c), _drMgr(dr), _drcMgr(drc) {}
  ~DrGridRoute() {}

  void solve();

 private:
  CirDB&    _cir;
  DrMgr&    _drMgr;
  DrcMgr&   _drcMgr;
};

PROJECT_NAMESPACE_END

#endif /// _DR_GRID_ROUTE_HPP_
