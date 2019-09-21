/**
 * @file   dbPin.hpp
 * @brief  Circuit Element - Pin
 * @author Hao Chen
 * @date   09/20/2019
 *
 **/

#ifndef _DB_PIN_HPP_
#define _DB_PIN_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START

class Pin {
  friend class Parser;
 public:
  Pin()
    : _name(""), _netName(""), _netIdx(MAX_INDEX) {}
  ~Pin() {}

  ////////////////////////////////////////
  //   Getter                           //
  ////////////////////////////////////////
  const String_t&                         name()                                const { return _name; }
  const String_t&                         netName()                             const { return _netName; }
  Index_t                                 netIdx()                              const { return _netIdx; }
  Index_t                                 layerIdx(const Index_t i)             const { return _vLayerIndices[i]; }
  Index_t                                 minLayerIdx()                         const { return _vLayerIndices.front(); }
  Index_t                                 maxLayerIdx()                         const { return _vLayerIndices.back(); }
  const Vector_t<Index_t>&                vLayerIndices()                       const { return _vLayerIndices; }
  const Box<Int_t>&                       box(const Index_t i, const Index_t j) const { return _vvBoxes[i][j]; }
  const Vector_t<Box<Int_t>>&             vBoxes(const Index_t i)               const { return _vvBoxes[i]; }
  const Vector_t<Vector_t<Box<Int_t>>>&   vvBoxes()                             const { return _vvBoxes; }

 private:
  String_t                        _name;
  String_t                        _netName;
  Index_t                         _netIdx;
  Vector_t<Index_t>               _vLayerIndices;
  Vector_t<Vector_t<Box<Int_t>>>  _vvBoxes; // Boxes in multiple layers
  
  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setName(const String_t& n) { _name = n; }
  void setNetName(const String_t& n) { _netName = n; }
};

PROJECT_NAMESPACE_END

#endif /// _DB_PIN_HPP_
