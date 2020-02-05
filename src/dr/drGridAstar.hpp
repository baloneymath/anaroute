/**
 * @file   drGridAstar.hpp
 * @brief  Detailed Routing - Grid-Based A* search kernel
 * @author Hao Chen
 * @date   02/05/2020
 *
 **/

#ifndef _DR_GRID_ASTAR_HPP_
#define _DR_GRID_ASTAR_HPP_

#include "drGridRoute.hpp"
#include "drGridAstarNode.hpp"
#include "src/ds/disjointSet.hpp"
#include "src/ds/hash.hpp"
#include "src/geo/point3d.hpp"
#include "src/geo/spatial.hpp"

PROJECT_NAMESPACE_START

class DrGridAstar {
 public:
  DrGridAstar(CirDB& c, Net& n, DrcMgr& d)
    : _cir(c), _net(n), _drc(d) {}
  ~DrGridAstar() {}

  bool run();

 private:
  CirDB&  _cir;
  Net&    _net;
  DrcMgr& _drc;
};

PROJECT_NAMESPACE_END

#endif /// _DR_GRID_ASTAR_HPP_
