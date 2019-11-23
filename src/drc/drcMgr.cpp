/**
 * @file   drcMgr.cpp
 * @brief  Design Rule Checking - Manager
 * @author Hao Chen
 * @date   10/23/2019
 *
 **/

#include "drcMgr.hpp"

PROJECT_NAMESPACE_START

/////////////////////////////////////////
//    Wire level checking              //
/////////////////////////////////////////
// short
bool DrcMgr::checkWireRoutingLayerShort(const UInt_t netIdx, const UInt_t layerIdx, const Box<Int_t>& b) const {
  // check pin
  Vector_t<UInt_t> vPinIndices;
  _cir.querySpatialPin(layerIdx, b, vPinIndices);
  for (const UInt_t pinIdx : vPinIndices) {
    const Pin& pin = _cir.pin(pinIdx);
    if (pin.netIdx() != netIdx)
      return false;
  }
  // check other net's wires
  Vector_t<UInt_t> vNetIndices;
  _cir.querySpatialRoutedWire(layerIdx, b, vNetIndices);
  for (const UInt_t idx : vNetIndices) {
    if (idx != netIdx)
      return false;
  }
  return true;
}

bool DrcMgr::checkWireCutLayerShort(const UInt_t netIdx, const UInt_t layerIdx, const Box<Int_t>& b) const {
  // no pin in cut layers
  // check other net's wire (via)
  Vector_t<UInt_t> vNetIndices;
  _cir.querySpatialRoutedWire(layerIdx, b, vNetIndices);
  for (const UInt_t idx : vNetIndices) {
    if (idx != netIdx)
      return false;
  }
  return true;
}

// min area
bool DrcMgr::checkWireMinArea(const UInt_t netIdx, const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

// spacing
bool DrcMgr::checkWireRoutingLayerSpacing(const UInt_t netIdx, const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

bool DrcMgr::checkWireCutLayerSpacing(const UInt_t netIdx, const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

bool DrcMgr::checkWireEolSpacing(const UInt_t netIdx, const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

PROJECT_NAMESPACE_END
