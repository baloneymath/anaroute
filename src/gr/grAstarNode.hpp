/**
 * @file   grAstarNode.hpp
 * @brief  Global Routing - Astar Node
 * @author Hao Chen
 * @date   10/09/2019
 *
 **/

#ifndef _GR_ASTAR_NODE_HPP_
#define _GR_ASTAR_NODE_HPP_

#include "src/global/global.hpp"
#include "src/geo/point3d.hpp"

PROJECT_NAMESPACE_START

class GrAstarNode {
 public:
  GrAstarNode(const Point3d<Int_t>& co = Point3d<Int_t>(),
              const Int_t g0 = MAX_INT, const Int_t g1 = MAX_INT,
              const Int_t f0 = MAX_INT, const Int_t f1 = MAX_INT,
              const Int_t b0 = 0, const Int_t b1 = 0,
              const bool e0 = false, const bool e1 = false,
              const UInt_t p0 = MAX_UINT, const UInt_t p1 = MAX_UINT)
    : _coord(co),
      _costG{g0, g1},
      _costF{f0, f1},
      _bendCnt{b0, b1},
      _bExplored{e0, e1},
      _parentNodeIdx{p0, p1}{}

  ~GrAstarNode() {}
  
  /////////////////////////////////////////
  //    Getters                          //
  /////////////////////////////////////////
  const Point3d<Int_t>&     coord()                       const { return _coord; }
  Int_t                     costG(const UInt_t i)         const { return _costG[i]; }
  Int_t                     costF(const UInt_t i)         const { return _costF[i]; }
  Int_t                     bendCnt(const UInt_t i)       const { return _bendCnt[i]; }
  bool                      bExplored(const UInt_t i)     const { return _bExplored[i]; }
  UInt_t                    parentNodeIdx(const UInt_t i) const { return _parentNodeIdx[i]; }
  Vector_t<UInt_t>&         vAdjNodeIndices()                   { return _vAdjNodeIndices; }
  const Vector_t<UInt_t>&   vAdjNodeIndices()             const { return _vAdjNodeIndices; }
  
  /////////////////////////////////////////
  //    Setters                          //
  /////////////////////////////////////////
  void setCoord(const Point3d<Int_t>& co) { _coord = co; }
  void setCostG(const UInt_t i, const Int_t c) { _costG[i] = c; }
  void setCostF(const UInt_t i, const Int_t c) { _costF[i] = c; }
  void setBendCnt(const UInt_t i, const Int_t c) { _bendCnt[i] = c; }
  void setExplored(const UInt_t i, const bool b) { _bExplored[i] = b; }
  void setParentNodeIdx(const UInt_t i, const UInt_t c) { _parentNodeIdx[i] = c; }
  void addAdjNodeIdx(const UInt_t i) { _vAdjNodeIndices.emplace_back(i); }
  void clearAdjNodeIndices() { _vAdjNodeIndices.clear(); }

 private:
  Point3d<Int_t>    _coord;
  Int_t             _costG[2];
  Int_t             _costF[2];
  Int_t             _bendCnt[2];
  bool              _bExplored[2];
  UInt_t            _parentNodeIdx[2];
  Vector_t<UInt_t>  _vAdjNodeIndices;
};

PROJECT_NAMESPACE_END

#endif /// _GR_ASTAR_NODE_HPP_
