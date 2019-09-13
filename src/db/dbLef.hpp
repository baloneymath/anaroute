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
#include "lef/lefLayers.hpp"

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
  Real_t                           version()           const { return _version; }
  const String_t&                  versionStr()        const { return _versionStr; }
  const String_t&                  dividerChar()       const { return _dividerChar; }
  Real_t                           manufacturingGrid() const { return _manufacturingGrid; }
  const String_t&                  clearanceMeasure()  const { return _clearanceMeasure; }
  const String_t&                  busbitChars()       const { return _busbitChars; }
  // units
  LefUnits&                        units()                   { return _units; }
  const LefUnits&                  units()             const { return _units; }
  
  // layers
  LefRoutingLayer&                 routingLayer(const Index_t i)       { return _vRoutingLayers[i]; }
  const LefRoutingLayer&           routingLayer(const Index_t i) const { return _vRoutingLayers[i]; }
  Vector_t<LefRoutingLayer>&       vRoutingLayers()                    { return _vRoutingLayers; }
  const Vector_t<LefRoutingLayer>& vRoutingLayers()              const { return _vRoutingLayers; }
  Index_t                          numRoutingLayers()            const { return _vRoutingLayers.size(); }

 private:
  // According to the Cadence Lef API
  Real_t                          _version;
  String_t                        _versionStr;
  String_t                        _dividerChar;
  Real_t                          _manufacturingGrid;
  String_t                        _clearanceMeasure;
  String_t                        _busbitChars;
  
  LefUnits                        _units;
  
  Vector_t<Pair_t<LayerType_t, Index_t>>  _vAllLayers; // first: type, second: idx in corresponding vector. ex. {ROUTING, 2} -> vRoutingLayers[2];
  Vector_t<LefRoutingLayer>               _vRoutingLayers;

  UMap_t<String_t, Index_t>       _mStr2RoutingLayerIdx;
  
  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setVersionStr(const String_t& v);
  void setVersion(const Real_t v);
  void setDividerChar(const String_t& v);
  void setManufacturingGrid(const Real_t v);
  void setClearanceMeasure(const String_t& v);
  void setBusbitChars(const String_t& v);
  void addRoutingLayer(const LefRoutingLayer& l);
};

PROJECT_NAMESPACE_END

#endif /// _DB_TECH_HPP_
