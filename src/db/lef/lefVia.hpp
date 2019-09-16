/**
 * @file   lefVia.hpp
 * @brief  Technology configuration - Lef Via
 * @author Hao Chen
 * @date   09/16/2019
 *
 **/

#ifndef _DB_LEF_VIA_HPP_
#define _DB_LEF_VIA_HPP_

#include "src/global/global.hpp"
#include "src/geo/box.hpp"

PROJECT_NAMESPACE_START

class LefVia {
  friend class LefReader;
 public:
  LefVia()
    :_name(""), _bDefault(false),
     _botLayerIdx(0), _cutLayerIdx(0), _topLayerIdx(0),
     _botLayerName(""), _cutLayerName(""), _topLayerName("") {}
  ~LefVia() {}

  /////////////////////////////////
  //    Getter                   //
  /////////////////////////////////
  const String_t&               name()                  const { return _name; }
  bool                          bDefault()              const { return _bDefault; }
  Index_t                       botLayerIdx()           const { return _botLayerIdx; }
  Index_t                       cutLayerIdx()           const { return _cutLayerIdx; }
  Index_t                       topLayerIdx()           const { return _topLayerIdx; }
  const String_t&               botLayerName()          const { return _botLayerName; } 
  const String_t&               cutLayerName()          const { return _cutLayerName; } 
  const String_t&               topLayerName()          const { return _topLayerName; }
  
  // Boxes
  Index_t                       numBotBoxes()           const { return _vBotBoxes.size(); }
  Index_t                       numCutBoxes()           const { return _vCutBoxes.size(); }
  Index_t                       numTopBoxes()           const { return _vTopBoxes.size(); }
  const Box<Int_t>&             botBox(const Index_t i) const { return _vBotBoxes[i]; }
  const Box<Int_t>&             cutBox(const Index_t i) const { return _vCutBoxes[i]; }
  const Box<Int_t>&             topBox(const Index_t i) const { return _vTopBoxes[i]; }
  const Vector_t<Box<Int_t>>&   vBotBoxes()             const { return _vBotBoxes; }
  const Vector_t<Box<Int_t>>&   vCutBoxes()             const { return _vCutBoxes; }
  const Vector_t<Box<Int_t>>&   vTopBoxes()             const { return _vTopBoxes; }
                                                                 
 private:                                                        
  String_t              _name;
  bool                  _bDefault; // true: fixed via, false: generated via
  Index_t               _botLayerIdx;
  Index_t               _cutLayerIdx;
  Index_t               _topLayerIdx;
  String_t              _botLayerName;
  String_t              _cutLayerName;
  String_t              _topLayerName;
  Vector_t<Box<Int_t>>  _vBotBoxes;
  Vector_t<Box<Int_t>>  _vCutBoxes;
  Vector_t<Box<Int_t>>  _vTopBoxes;
  /////////////////////////////////
  //    Setter                   //
  /////////////////////////////////
  void setName(const String_t& n);
  void setDefault();
  void setBotLayerIdx(const Index_t i);
  void setCutLayerIdx(const Index_t i);
  void setTopLayerIdx(const Index_t i);
  void setBotLayerName(const String_t& n);
  void setCutLayerName(const String_t& n);
  void setTopLayerName(const String_t& n);
  void addBotBox(const Box<Int_t>& b);
  void addCutBox(const Box<Int_t>& b);
  void addTopBox(const Box<Int_t>& b);
};

PROJECT_NAMESPACE_END

#endif /// _DB_LEF_VIA_HPP_
