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
bool DrcMgr::checkWireRoutingLayerShort(const UInt_t layerIdx, const Box<Int_t>& b) const {
  const SpatialMap<Int_t, UInt_t>& routedWireSpatial = _vSpatialRoutedWires[layerIdx];
  return true;
}

bool DrcMgr::checkWireCutLayerShort(const UInt_t layerIdx, const Box<Int_t>& b) const {
  return true;
}

// min width
bool DrcMgr::checkWireMinWidth(const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

// min area
bool DrcMgr::checkWireMinArea(const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

// spacing
bool DrcMgr::checkWireRoutingLayerSpacing(const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

bool DrcMgr::checkWireCutLayerSpacing(const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

bool DrcMgr::checkWireEolSpacing(const UInt_t layerIdx, const Box<Int_t>& b) const {

  return true;
}

PROJECT_NAMESPACE_END
