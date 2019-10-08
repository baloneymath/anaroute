/**
 * @file   grGridMap3d.hpp
 * @brief  Global Routing Structure - 3D GridMap
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#ifndef _GR_GRID_MAP_3D_HPP_
#define _GR_GRID_MAP_3D_HPP_

#include "grGridMap2d.hpp"

PROJECT_NAMESPACE_START

class GrGridMap3d {
 public:
  GrGridMap3d() {}
  ~GrGridMap3d() {}

  /////////////////////////////////////////
  //    Getters                          //
  /////////////////////////////////////////
  UInt_t              numGridCellsX(const UInt_t i)                             const { return _vGridMaps[i].numGridCellsX(); }
  UInt_t              numGridCellsY(const UInt_t i)                             const { return _vGridMaps[i].numGridCellsY(); }
  UInt_t              numGridCellsZ()                                           const { return _vGridMaps.size(); }
  GrGridCell&         gridCell(const UInt_t z, const UInt_t x, const UInt_t y)        { return _vGridMaps[z].gridCell(x, y); }
  const GrGridCell&   gridCell(const UInt_t z, const UInt_t x, const UInt_t y)  const { return _vGridMaps[z].gridCell(x, y); }
  GrGridMap2d&        gridMap(const UInt_t i)                                         { return _vGridMaps[i]; } 
  const GrGridMap2d&  gridMap(const UInt_t i)                                   const { return _vGridMaps[i]; } 

  /////////////////////////////////////////
  //    Setters                          //
  /////////////////////////////////////////
  void setNumZ(const UInt_t i) { _vGridMaps.resize(i); }
  void setNumX(const UInt_t i, const UInt_t x) { _vGridMaps[i].setNumX(x); }
  void setNumY(const UInt_t i, const UInt_t y) { _vGridMaps[i].setNumY(y); }
  void setGridCell(const UInt_t z, const UInt_t x, const UInt_t y, const GrGridCell& c) { _vGridMaps[z].setGridCell(x, y, c); }
 
 private:
  Vector_t<GrGridMap2d> _vGridMaps;
};

PROJECT_NAMESPACE_END

#endif /// _GR_GRID_MAP_3D_HPP_
