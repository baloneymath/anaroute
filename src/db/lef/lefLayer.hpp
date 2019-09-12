/**
 * @file   lefLayer.hpp
 * @brief  Technology configuration - Lef layer
 * @author Hao Chen
 * @date   09/11/2019
 *
 **/


#ifndef _DB_LEF_LAYER_HPP_
#define _DB_LEF_LAYER_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START


struct LefSpacingTable {
  Vector_t<Int_t> vParallelRunLength; // v[0] = runlength0, v[1] = runlength1 ...
  Vector_t<Pair_t<Int_t, Vector_t<Int_t>>> table; // table[0] = {width, {spacing 0, spacing1...}}
};


class LefLayer {
  friend class LefReader;

 public:
  enum class RouteDir : Byte_t {
    HORIZONTAL = 0,
    VERTICAL   = 1,
    BOTH       = 2,
    POLY       = 3,
    FORBID     = 4
    //DIAG45
    //DIAG135
  };

  LefLayer()
    : _name(""), _type(""), _routeDirStr(""), _routeDir(RouteDir::FORBID),
      _minArea(0), _minWidth(0), _defaultWidth(0), _spacing(0),
      _eolSpacing(0), _eolWidth(0), _eolWithin(0),
      _pitchX(0), _pitchY(0) {}
  ~LefLayer() {}

  ////////////////////////////////////////
  //   Getter                           //
  ////////////////////////////////////////
  const String_t&         name()          const { return _name; }
  const String_t&         type()          const { return _type; }
  const String_t&         routeDirStr()   const { return _routeDirStr; }
  RouteDir                routeDir()      const { return _routeDir; }
  Int_t                   minArea()       const { return _minArea; }
  Int_t                   minWidth()      const { return _minWidth; }
  Int_t                   defaultWidth()  const { return _defaultWidth; }
  Int_t                   spacing()       const { return _spacing; }
  Int_t                   eolSpacing()    const { return _eolSpacing; }
  Int_t                   eolWidth()      const { return _eolWidth; }
  Int_t                   eolWithin()     const { return _eolWithin; }
  Int_t                   pitchX()        const { return _pitchX; }
  Int_t                   pitchY()        const { return _pitchY; }
  const LefSpacingTable&  spacingTable()  const { return _spacingTable; }

 private:
  //////////////////////////////////////
  //    Members                       //
  //////////////////////////////////////
  String_t          _name;
  String_t          _type;
  String_t          _routeDirStr;
  RouteDir          _routeDir;
  Int_t             _minArea;
  Int_t             _minWidth;
  Int_t             _defaultWidth;
  Int_t             _spacing;
  Int_t             _eolSpacing;
  Int_t             _eolWidth;
  Int_t             _eolWithin;
  Int_t             _pitchX;
  Int_t             _pitchY;
  LefSpacingTable   _spacingTable;

  //////////////////////////////////////
  //    Setter                        //
  //////////////////////////////////////
  void setName(const String_t& n) { _name = n; }
  void setType(const String_t& t) { _type = t; }
  void setRouteDir(const String_t& d) {
    _routeDirStr = d;
    if (d == "HORIZONTAL") _routeDir = RouteDir::HORIZONTAL;
    else if (d == "VERTICAL") _routeDir = RouteDir::VERTICAL;
    else assert(false);
  }
  void setMinArea(const Int_t a) { _minArea = a; }
  void setMinWidth(const Int_t w) { _minWidth = w; }
  void setDefaultWidth(const Int_t w) { _defaultWidth = w; }
  void setSpacing(const Int_t s) { _spacing = s; }
  void setEolSpacing(const Int_t s) { _eolSpacing = s; }
  void setEolWidth(const Int_t w) { _eolWidth = w; }
  void setEolWithin(const Int_t w) { _eolWithin = w; }
  void setPitchX(const Int_t p) { _pitchX = p; }
  void setPitchY(const Int_t p) { _pitchY = p; }
  
  void addParallelRunLength(const Int_t p) { _spacingTable.vParallelRunLength.emplace_back(p); }
  void addSpacingTableWidth(const Int_t w) { _spacingTable.table.emplace_back(w, Vector_t<Int_t>()); }
  void addSpacingTableWidthSpacing(const Index_t idx, const Int_t s) { _spacingTable.table[idx].second.emplace_back(s); }
};


PROJECT_NAMESPACE_END

#endif /// _DB_LEF_LAYER_HPP_
