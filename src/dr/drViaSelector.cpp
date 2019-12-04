/**
 * @file   drViaSelector.cpp
 * @brief  Detailed Routing - Via Selector
 * @author Hao Chen
 * @date   10/27/2019
 *
 **/

#include "drViaSelector.hpp"

PROJECT_NAMESPACE_START

UInt_t DrViaSelector::selectViaIdx(const UInt_t netIdx, const Point3d<Int_t>& u, const Point3d<Int_t>& v) {
  const Int_t zl = std::min(u.z(), v.z());
  const Int_t zc = zl + 1;
  const Int_t zh = std::max(u.z(), v.z());
  assert(zh - zl == 2);
  assert(u.x() == v.x() and u.y() == v.y());
  const Int_t x = u.x();
  const Int_t y = u.y();
  UInt_t i, j, ret = MAX_INT;
  UInt_t maxIdx = _cir.lef().numVias();
  for (i = 0; i < maxIdx; ++i) {
    bool bViolate = false;
    const LefVia& lefVia = _cir.lef().via(i);
    for (j = 0; j < lefVia.numBotBoxes(); ++j) {
      Box<Int_t> box(lefVia.botBox(j));
      box.shift(x, y);
      //if (!_drcMgr.checkWireRoutingLayerShort(netIdx, zl, box)) {
        //bViolate = true;
        //break;
      //}
      if (!_drcMgr.checkWireRoutingLayerSpacing(netIdx, zl, box)) {
        bViolate = true;
        break;
      }
      if (!_drcMgr.checkWireEolSpacing(netIdx, zl, box)) {
        bViolate = true;
        break;
      }
    }
    if (bViolate)
      continue;
    for (j = 0; j < lefVia.numCutBoxes(); ++j) {
      Box<Int_t> box(lefVia.cutBox(j));
      box.shift(x, y);
      if (!_drcMgr.checkWireCutLayerShort(netIdx, zc, box)) {
        bViolate = true;
        break;
      }
      if (!_drcMgr.checkWireCutLayerSpacing(netIdx, zc, box)) {
        bViolate = true;
        break;
      }
    }
    if (bViolate)
      continue;
    for (j = 0; j < lefVia.numTopBoxes(); ++j) {
      Box<Int_t> box(lefVia.topBox(j));
      box.shift(x, y);
      //if (!_drcMgr.checkWireRoutingLayerShort(netIdx, zh, box)) {
        //bViolate = true;
        //break;
      //}
      if (!_drcMgr.checkWireRoutingLayerSpacing(netIdx, zh, box)) {
        bViolate = true;
        break;
      }
      if (!_drcMgr.checkWireEolSpacing(netIdx, zh, box)) {
        bViolate = true;
        break;
      }
    }
    if (bViolate)
      continue;
    ret = i;
    break;
  }
  return ret;
}

PROJECT_NAMESPACE_END
