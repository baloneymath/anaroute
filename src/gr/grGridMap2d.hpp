/**
 * @file   grGridMap2d.hpp
 * @brief  Global Routing Structure - 2D GridMap
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#ifndef _GR_GRID_MAP_2D_HPP_
#define _GR_GRID_MAP_2D_HPP_

#include "grGridCell.hpp"

PROJECT_NAMESPACE_START

class GrGridMap2d {
 public:
  GrGridMap2d() {}
  ~GrGridMap2d() {}

  /////////////////////////////////////////
  //    Getters                          //
  /////////////////////////////////////////
  UInt_t              numGridCellsX()                          const  { return _vvGridCells.size(); }
  UInt_t              numGridCellsY()                          const  { return _vvGridCells[0].size(); }
  GrGridCell&         gridCell(const UInt_t i, const UInt_t j)        { return _vvGridCells[i][j]; }
  const GrGridCell&   gridCell(const UInt_t i, const UInt_t j) const  { return _vvGridCells[i][j]; }
  
  /////////////////////////////////////////
  //    Setters                          //
  /////////////////////////////////////////
  void setNumX(const UInt_t i) { _vvGridCells.resize(i); }
  void setNumY(const UInt_t i) { for (auto& vec : _vvGridCells) vec.resize(i); }
  void setGridCell(const UInt_t i, const UInt_t j, const GrGridCell& c) { _vvGridCells[i][j] = c; }

 private:
  Vector_t<Vector_t<GrGridCell>> _vvGridCells;
};

PROJECT_NAMESPACE_END

#endif /// _GR_GRID_MAP_2D_HPP_
