/**
 * @file   grAstar.hpp
 * @brief  Global Routing - Astar Kernel
 * @author Hao Chen
 * @date   10/09/2019
 *
 **/

#ifndef _GR_ASTAR_HPP_
#define _GR_ASTAR_HPP_

#include "grGridRoute.hpp"
#include "grAstarNode.hpp"
#include "src/ds/disjointSet.hpp"
#include "src/ds/hash.hpp"

PROJECT_NAMESPACE_START

class GrAstar {
 public:
  GrAstar(CirDB& c, Net& n, GrGridRoute& g)
    : _cir(c), _net(n), _grGridRoute(g) {}
  ~GrAstar() {}
  
  void runKernel();

 private:
  CirDB&        _cir;
  Net&          _net;
  GrGridRoute&  _grGridRoute;
  
  /////////////////////////////////////////
  //    Private structs                  //
  /////////////////////////////////////////
  struct NetComps {
    DisjointSet                                                     compUnion;
    Vector_t<Box<Int_t>>                                            vCompBoxes;
    Vector_t<UInt_t>                                                vCompBoxLayerIndices;
    Vector_t<DenseHashSet<Point3d<Int_t>, Point3d<Int_t>::hasher>>  vCompDatas;

    void clear() {
      vCompBoxes.clear();
      vCompBoxLayerIndices.clear();
      vCompDatas.clear();
    }
  } _nc;
};

PROJECT_NAMESPACE_END

#endif /// _GR_ASTAR_HPP_
