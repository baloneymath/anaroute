/**
 * @file   drAstar.hpp
 * @brief  Detailed Routing - A* search kernel
 * @author Hao Chen
 * @date   10/25/2019
 *
 **/

#ifndef _DR_ASTAR_HPP_
#define _DR_ASTAR_HPP_

#include "drGridlessRoute.hpp"
#include "drAstarNode.hpp"
#include "src/ds/disjointSet.hpp"
#include "src/ds/hash.hpp"
#include "src/geo/point3d.hpp"
#include "src/geo/spatial.hpp"

PROJECT_NAMESPACE_START

class DrAstar {
 public:
  DrAstar(CirDB& c, Net& n, DrGridlessRoute& d)
    : _cir(c), _net(n), _drGridlessRoute(d), _drcMgr(d._drcMgr),
      _vSpatialNetGuides(c.vSpatialNetGuides(n.idx())) {}
  ~DrAstar() {}

  bool runKernel();

 private:
  CirDB&            _cir;
  Net&              _net;
  DrGridlessRoute&  _drGridlessRoute;
  DrcMgr&           _drcMgr;
  
  const Vector_t<Spatial<Int_t>>& _vSpatialNetGuides;

  DisjointSet                                                     _compDS;
  Vector_t<Vector_t<Pair_t<Box<Int_t>, Int_t>>>                   _vCompBoxes;
  Vector_t<DenseHashSet<Point3d<Int_t>, Point3d<Int_t>::hasher>>  _vCompAcsPts;
  Vector_t<Pair_t<UInt_t, UInt_t>>                                _vCompPairs;
  
  struct Param {
    Int_t horCost = 1;
    Int_t verCost = 1;
    Int_t viaCost = 1000;
    Int_t factorG = 1;
    Int_t factorH = 1;
  } _param;

  // for self sym
  Int_t _selfSymAxisX = 0;

  // for sym
  Int_t _symAxisX = 0;

  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
  void splitSubNetMST();
  void initAcsPoints();
  bool routeSubNet(UInt_t srcIdx, UInt_t tarIdx);
  bool computeSelfSymAxisX();
  bool computeSymAxisX();
  bool bSatisfySelfSymCondition() const;
  bool bSatisfySymCondition() const;

  /////////////////////////////////////////
  //    Helper functions                 //
  /////////////////////////////////////////
  Int_t scaledMDist(const Point3d<Int_t>& u, const Point3d<Int_t>& v);
  Int_t scaledMDist(const Box<Int_t>& u, const Box<Int_t>& v);
  Int_t scaledMDist(const Pair_t<Box<Int_t>, Int_t>& u, const Pair_t<Box<Int_t>, Int_t>& v);

};

PROJECT_NAMESPACE_END

#endif /// _DR_ASTAR_HPP_
