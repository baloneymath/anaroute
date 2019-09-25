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
#include "lef/lefSite.hpp"
#include "lef/lefVia.hpp"

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
  const LefUnits&                  units()             const { return _units; }
  
  ////////////////////////////////////////
  //   Layers                           //
  ////////////////////////////////////////
  // All
  Index_t                                  numLayers()                                 const { return _vAllLayers.size(); }
  Index_t                                  str2LayerIdx(const String_t& n)             const { return _mStr2AllLayerIdx.at(n); }
  const Pair_t<LefLayerType, Index_t>&     str2Layer(const String_t& n)                const { return _vAllLayers[str2LayerIdx(n)]; }
  bool                                     bImplantLayer(const Index_t i)              const { return _vAllLayers[i].first == LefLayerType::IMPLANT; }
  bool                                     bMastersliceLayer(const Index_t i)          const { return _vAllLayers[i].first == LefLayerType::MASTERSLICE; }
  bool                                     bCutLayer(const Index_t i)                  const { return _vAllLayers[i].first == LefLayerType::CUT; }
  bool                                     bRoutingLayer(const Index_t i)              const { return _vAllLayers[i].first == LefLayerType::ROUTING; }
  bool                                     bOverlapLayer(const Index_t i)              const { return _vAllLayers[i].first == LefLayerType::OVERLAP; }
  // Implant Layers                                                                    
  const LefImplantLayer&                   implantLayer(const Index_t i)               const { return _vImplantLayers[i]; }
  const Vector_t<LefImplantLayer>&         vImplantLayers()                            const { return _vImplantLayers; }
  Index_t                                  numImplantLayers()                          const { return _vImplantLayers.size(); }
  Index_t                                  str2ImplantLayerIdx(const String_t& n)      const { return _mStr2ImplantLayerIdx.at(n); }
  const LefImplantLayer&                   str2ImplantLayer(const String_t& n)         const { return _vImplantLayers[str2ImplantLayerIdx(n)]; }
  // Masterslice Layers
  const LefMastersliceLayer&               mastersliceLayer(const Index_t i)           const { return _vMastersliceLayers[i]; }
  const Vector_t<LefMastersliceLayer>&     vMastersliceLayers()                        const { return _vMastersliceLayers; }
  Index_t                                  numMastersliceLayers()                      const { return _vMastersliceLayers.size(); }
  Index_t                                  str2MastersliceLayerIdx(const String_t& n)  const { return _mStr2MastersliceLayerIdx.at(n); }
  const LefMastersliceLayer&               str2MastersliceLayer(const String_t& n)     const { return _vMastersliceLayers[str2MastersliceLayerIdx(n)]; }
  // Cut Layers
  const LefCutLayer&                       cutLayer(const Index_t i)                   const { return _vCutLayers[i]; }
  const Vector_t<LefCutLayer>&             vCutLayers()                                const { return _vCutLayers; }
  Index_t                                  numCutLayers()                              const { return _vCutLayers.size(); }
  Index_t                                  str2CutLayerIdx(const String_t& n)          const { return _mStr2CutLayerIdx.at(n); }
  const LefCutLayer&                       str2CutLayer(const String_t& n)             const { return _vCutLayers[str2CutLayerIdx(n)]; }
  // Routing layers                                                          
  const LefRoutingLayer&                   routingLayer(const Index_t i)               const { return _vRoutingLayers[i]; }
  const Vector_t<LefRoutingLayer>&         vRoutingLayers()                            const { return _vRoutingLayers; }
  Index_t                                  numRoutingLayers()                          const { return _vRoutingLayers.size(); }
  Index_t                                  str2RoutinglayerIdx(const String_t& n)      const { return _mStr2RoutingLayerIdx.at(n); }
  const LefRoutingLayer&                   str2Routinglayer(const String_t& n)         const { return _vRoutingLayers[str2RoutinglayerIdx(n)]; }
  // Overlap layers                                                          
  const LefOverlapLayer&                   overlapLayer(const Index_t i)               const { return _vOverlapLayers[i]; }
  const Vector_t<LefOverlapLayer>&         vOverlapLayers()                            const { return _vOverlapLayers; }
  Index_t                                  numOverlapLayers()                          const { return _vOverlapLayers.size(); }
  Index_t                                  str2OverlaplayerIdx(const String_t& n)      const { return _mStr2OverlapLayerIdx.at(n); }
  const LefOverlapLayer&                   str2Overlaplayer(const String_t& n)         const { return _vOverlapLayers[str2OverlaplayerIdx(n)]; }
  ////////////////////////////////////////
  //   Vias                             //
  ////////////////////////////////////////
  const LefVia&                   via(const Index_t i) const { return _vVias[i]; }
  const Vector_t<LefVia>&         vVias()              const { return _vVias; }
  Index_t                         numVias()            const { return _vVias.size(); }

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
  
  // unit
  LefUnits                        _units;
  
  // site
  Vector_t<LefSite>              _vSites;
  UMap_t<String_t, Index_t>      _mStr2SiteIdx;

  // Layer
  Vector_t<Pair_t<LefLayerType, Index_t>>  _vAllLayers; // first: type, second: idx in corresponding vector. ex. {ROUTING, 2} -> vRoutingLayers[2];
  Vector_t<LefImplantLayer>                _vImplantLayers;
  Vector_t<LefMastersliceLayer>            _vMastersliceLayers;
  Vector_t<LefCutLayer>                    _vCutLayers;
  Vector_t<LefRoutingLayer>                _vRoutingLayers;
  Vector_t<LefOverlapLayer>                _vOverlapLayers;
  UMap_t<String_t, Index_t>                _mStr2AllLayerIdx;
  UMap_t<String_t, Index_t>                _mStr2ImplantLayerIdx;
  UMap_t<String_t, Index_t>                _mStr2MastersliceLayerIdx;
  UMap_t<String_t, Index_t>                _mStr2CutLayerIdx;
  UMap_t<String_t, Index_t>                _mStr2RoutingLayerIdx;
  UMap_t<String_t, Index_t>                _mStr2OverlapLayerIdx;
  
  // Via
  Vector_t<LefVia>              _vVias;
  UMap_t<String_t, Index_t>     _mStr2ViaIdx;

  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setVersionStr(const String_t& v);
  void setVersion(const Real_t v);
  void setDividerChar(const String_t& v);
  void setManufacturingGrid(const Real_t v);
  void setClearanceMeasure(const String_t& v);
  void setBusbitChars(const String_t& v);
  
  void addSite(const LefSite& s);

  void addImplantLayer(const LefImplantLayer& l);
  void addMastersliceLayer(const LefMastersliceLayer& l);
  void addCutLayer(const LefCutLayer& l);
  void addRoutingLayer(const LefRoutingLayer& l);
  void addOverlapLayer(const LefOverlapLayer& l);

  void addVia(const LefVia& v);
};

PROJECT_NAMESPACE_END

#endif /// _DB_TECH_HPP_
