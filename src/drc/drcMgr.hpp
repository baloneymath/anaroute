/**
 * @file   drcMgr.hpp
 * @brief  Design Rule Checking - Manager
 * @author Hao Chen
 * @date   10/23/2019
 *
 **/

#ifndef _DRC_MGR_HPP_
#define _DRC_MGR_HPP_

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"
#include "src/geo/box.hpp"
#include "src/geo/spatial.hpp"
#include "src/geo/spatial3d.hpp"

PROJECT_NAMESPACE_START

class DrcMgr {
 public:
  DrcMgr(CirDB& c)
    : _cir(c), _vSpatialPins(c.vSpatialPins()), _vSpatialBlks(c.vSpatialBlks()),
      _vSpatialRoutedWires(c.vSpatialRoutedWires()) {}
  ~DrcMgr() {}

  /////////////////////////////////////////
  //    Wire level checking              //
  /////////////////////////////////////////
  // short
  bool checkWireRoutingLayerShort(const Box<Int_t>& b);
  bool checkWireCutLayerShort(const Box<Int_t>& b);
  // min width
  bool checkWireMinWidth(const Box<Int_t>& b);
  // min area
  bool checkWireMinArea(const Box<Int_t>& b);
  // spacing
  bool checkWireRoutingLayerSpacing(const Box<Int_t>& b);
  bool checkWireCutLayerSpacing(const Box<Int_t>& b);
  bool checkWireEolSpacing(const Box<Int_t>& b);
  
  /////////////////////////////////////////
  //    Net level checking               //
  /////////////////////////////////////////

 private:
  CirDB& _cir;
  const Vector_t<SpatialMap<Int_t, UInt_t>>&  _vSpatialPins;
  const Vector_t<SpatialMap<Int_t, UInt_t>>&  _vSpatialBlks;
  const Vector_t<SpatialMap<Int_t, UInt_t>>&  _vSpatialRoutedWires;

};

PROJECT_NAMESPACE_END

#endif /// _DRC_MGR_HPP_
