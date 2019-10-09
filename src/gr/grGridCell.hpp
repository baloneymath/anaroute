/**
 * @file   grGridCell.hpp
 * @brief  Global Routing Structure - GridCell
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/


#ifndef _GR_GRID_CELL_HPP_
#define _GR_GRID_CELL_HPP_

#include "src/global/global.hpp"
#include "src/geo/point3d.hpp"

PROJECT_NAMESPACE_START

class GrGridCell {
 public:
  GrGridCell()
    : _width(0), _height(0), _bValid(true) {}
  ~GrGridCell() {}

  /////////////////////////////////////////
  //    Getters                          //
  /////////////////////////////////////////
  Point3d<Int_t>            loc()                   const { return _loc; }
  Int_t                     x()                     const { return _loc.x(); }
  Int_t                     y()                     const { return _loc.y(); }
  Int_t                     z()                     const { return _loc.z(); }
  Int_t                     width()                 const { return _width; }
  Int_t                     height()                const { return _height; }
  UInt_t                    pinIdx(const UInt_t i)  const { return _vPinIndices[i]; }
  UInt_t                    numPins()               const { return _vPinIndices.size(); }
  const Vector_t<UInt_t>&   vPinIndices()           const { return _vPinIndices; }
  bool                      bValid()                const { return _bValid; }

  /////////////////////////////////////////
  //    Setters                          //
  /////////////////////////////////////////
  void setLoc(const Point3d<Int_t>& p)                      { _loc = p; }
  void setLoc(const Int_t x, const Int_t y, const Int_t z)  { _loc.setXYZ(x, y, z); }
  void setWidth(const Int_t w)                              { _width = w; }
  void setHeight(const Int_t h)                             { _height = h; }
  void addPinIdx(const UInt_t i)                            { _vPinIndices.emplace_back(i); }
  void setValid()                                           { _bValid = true; }
  void setInvalid()                                         { _bValid = false; }

 private:
  Point3d<Int_t>    _loc; // center of the grid
  Int_t             _width;
  Int_t             _height;
  Vector_t<UInt_t>  _vPinIndices; // pins located in this grid
  bool              _bValid;
};

PROJECT_NAMESPACE_END

#endif /// _GR_GRID_CELL_HPP_
