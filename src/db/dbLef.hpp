/**
 * @file   dbLef.hpp
 * @brief  Technology configuration from Lef file
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#ifndef _DB_TECH_HPP_
#define _DB_TECH_HPP_

#include <limbo/parsers/lef/adapt/LefDataBase.h> // for class lefiXXX

#include "src/global/global.hpp"
#include "lef/lefUnits.hpp"
#include "lef/lefLayer.hpp"

PROJECT_NAMESPACE_START

class LefDB {
  friend class LefReader;
 public:
  LefDB()
    : _version(0), _versionStr(""), _dividerChar(""), _manufacturingGrid(0),
      _clearanceMeasure(""), _busbitChars("") {}
  ~LefDB() {}
  
  ////////////////////////////////////////
  //   Getter                           //
  ////////////////////////////////////////
  Real_t                    version()           const { return _version; }
  const String_t&           versionStr()        const { return _versionStr; }
  const String_t&           dividerChar()       const { return _dividerChar; }
  Real_t                    manufacturingGrid() const { return _manufacturingGrid; }
  const String_t&           clearanceMeasure()  const { return _clearanceMeasure; }
  const String_t&           busbitChars()       const { return _busbitChars; }
  LefUnits&                 units()                   { return _units; }
  const LefUnits&           units()             const { return _units; }
  Vector_t<LefLayer>&       vLayers()                 { return _vLayers; }
  const Vector_t<LefLayer>& vLayers()           const { return _vLayers; }

 private:
  // According to the Cadence Lef API
  Real_t                   _version;
  String_t                 _versionStr;
  String_t                 _dividerChar;
  Real_t                   _manufacturingGrid;
  String_t                 _clearanceMeasure;
  String_t                 _busbitChars;
  LefUnits                 _units;
  Vector_t<LefLayer>       _vLayers;
  
  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setVersionStr(const String_t& v) { _versionStr = v; }
  void setVersion(const Real_t v) { _version = v; }
  void setDividerChar(const String_t& v) { _dividerChar = v; }
  void setManufacturingGrid(const Real_t v) { _manufacturingGrid = v; }
  void setClearanceMeasure(const String_t& v) { _clearanceMeasure = v; }
  void setBusbitChars(const String_t& v) { _busbitChars = v; }
};

PROJECT_NAMESPACE_END

#endif /// _DB_TECH_HPP_
