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
  //   Basics                           //
  ////////////////////////////////////////
  Real_t                           version()           const { return _version; }
  const String_t&                  versionStr()        const { return _versionStr; }
  const String_t&                  dividerChar()       const { return _dividerChar; }
  Real_t                           manufacturingGrid() const { return _manufacturingGrid; }
  const String_t&                  clearanceMeasure()  const { return _clearanceMeasure; }
  const String_t&                  busbitChars()       const { return _busbitChars; }

  ////////////////////////////////////////
  //   Units                            //
  ////////////////////////////////////////
  LefUnits&                        units()                   { return _units; }
  const LefUnits&                  units()             const { return _units; }
  
  
  ////////////////////////////////////////
  //   Layers                           //
  ////////////////////////////////////////
  // Implant Layers
  LefImplantLayer&                         implantLayer(const Index_t i)       { return _vImplantLayers[i]; }
  const LefImplantLayer&                   implantLayer(const Index_t i) const { return _vImplantLayers[i]; }
  Vector_t<LefImplantLayer>&               vImplantLayers()                    { return _vImplantLayers; }
  const Vector_t<LefImplantLayer>&         vImplantLayers()              const { return _vImplantLayers; }
  Index_t                                  numImplantLayers()            const { return _vImplantLayers.size(); }
  // Masterslice Layers
  LefMastersliceLayer&                     mastersliceLayer(const Index_t i)       { return _vMastersliceLayers[i]; }
  const LefMastersliceLayer&               mastersliceLayer(const Index_t i) const { return _vMastersliceLayers[i]; }
  Vector_t<LefMastersliceLayer>&           vMastersliceLayers()                    { return _vMastersliceLayers; }
  const Vector_t<LefMastersliceLayer>&     vMastersliceLayers()              const { return _vMastersliceLayers; }
  Index_t                                  numMastersliceLayers()            const { return _vMastersliceLayers.size(); }
  // Cut Layers
  LefCutLayer&                             cutLayer(const Index_t i)       { return _vCutLayers[i]; }
  const LefCutLayer&                       cutLayer(const Index_t i) const { return _vCutLayers[i]; }
  Vector_t<LefCutLayer>&                   vCutLayers()                    { return _vCutLayers; }
  const Vector_t<LefCutLayer>&             vCutLayers()              const { return _vCutLayers; }
  Index_t                                  numCutLayers()            const { return _vCutLayers.size(); }
  // Routing layers                        
  LefRoutingLayer&                         routingLayer(const Index_t i)       { return _vRoutingLayers[i]; }
  const LefRoutingLayer&                   routingLayer(const Index_t i) const { return _vRoutingLayers[i]; }
  Vector_t<LefRoutingLayer>&               vRoutingLayers()                    { return _vRoutingLayers; }
  const Vector_t<LefRoutingLayer>&         vRoutingLayers()              const { return _vRoutingLayers; }
  Index_t                                  numRoutingLayers()            const { return _vRoutingLayers.size(); }
  // Overlap layers                        
  LefOverlapLayer&                         overlapLayer(const Index_t i)       { return _vOverlapLayers[i]; }
  const LefOverlapLayer&                   overlapLayer(const Index_t i) const { return _vOverlapLayers[i]; }
  Vector_t<LefOverlapLayer>&               vOverlapLayers()                    { return _vOverlapLayers; }
  const Vector_t<LefOverlapLayer>&         vOverlapLayers()              const { return _vOverlapLayers; }
  Index_t                                  numOverlapLayers()            const { return _vOverlapLayers.size(); }
  
  // for debug
  void logInfo() const;

 private:
  // According to the Cadence Lef API
  Real_t                          _version;
  String_t                        _versionStr;
  String_t                        _dividerChar;
  Real_t                          _manufacturingGrid;
  String_t                        _clearanceMeasure;
  String_t                        _busbitChars;
  
  LefUnits                        _units;
  
  // Layers
  Vector_t<Pair_t<LayerType_t, Index_t>>  _vAllLayers; // first: type, second: idx in corresponding vector. ex. {ROUTING, 2} -> vRoutingLayers[2];
  Vector_t<LefImplantLayer>               _vImplantLayers;
  Vector_t<LefMastersliceLayer>           _vMastersliceLayers;
  Vector_t<LefCutLayer>                   _vCutLayers;
  Vector_t<LefRoutingLayer>               _vRoutingLayers;
  Vector_t<LefOverlapLayer>               _vOverlapLayers;
  UMap_t<String_t, Index_t>       _mStr2ImplantLayerIdx;
  UMap_t<String_t, Index_t>       _mStr2MastersliceLayerIdx;
  UMap_t<String_t, Index_t>       _mStr2CutLayerIdx;
  UMap_t<String_t, Index_t>       _mStr2RoutingLayerIdx;
  UMap_t<String_t, Index_t>       _mStr2OverlapLayerIdx;
  
  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setVersionStr(const String_t& v);
  void setVersion(const Real_t v);
  void setDividerChar(const String_t& v);
  void setManufacturingGrid(const Real_t v);
  void setClearanceMeasure(const String_t& v);
  void setBusbitChars(const String_t& v);
  void addImplantLayer(const LefImplantLayer& l);
  void addMastersliceLayer(const LefMastersliceLayer& l);
  void addCutLayer(const LefCutLayer& l);
  void addRoutingLayer(const LefRoutingLayer& l);
  void addOverlapLayer(const LefOverlapLayer& l);
};

PROJECT_NAMESPACE_END

#endif /// _DB_TECH_HPP_
