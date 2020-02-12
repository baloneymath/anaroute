/**
 * @file   acsMgr.cpp
 * @brief  Access points manager
 * @author Keren Zhu
 * @date   02/05/2020
 *
 **/

#include "acsMgr.hpp"

PROJECT_NAMESPACE_START

void AcsMgr::computeAcs() {
  fprintf(stdout, "AcsMgr::%s Start Access Points Generation\n", __func__);
  for (UInt_t pinIdx = 0; pinIdx < _cir.numPins(); ++pinIdx) {
    computePinAcs(pinIdx);
  }
}

struct CandidateAcs {
 public:
  explicit CandidateAcs(Int_t x, Int_t y, Int_t z, UInt_t boxIdx_)
    : pt(x, y, z), boxIdx(boxIdx_) {}
  Point3d<Int_t>  pt; ///< The 3D point for representing the grid indices
  UInt_t          boxIdx; ///< The index of the box this point is in. pin.box(pt.z(), boxIdx.y())
};

void AcsMgr::computePinAcs(const UInt_t pinIdx) {
  auto& pin = _cir.pin(pinIdx);
  Vector_t<CandidateAcs> candidates; // The candidates for pin access
  for (UInt_t layerIdx = pin.minLayerIdx(); layerIdx <= pin.maxLayerIdx(); ++layerIdx) {
    for (UInt_t i = 0; i < pin.numBoxes(layerIdx); ++i) {
      const auto& box = pin.box(layerIdx, i);
      Vector_t<Point3d<Int_t>> vAcs;
      computeBoxAcs(box, layerIdx, vAcs);
      for (const auto& p : vAcs) {
        candidates.emplace_back(CandidateAcs(p.x(), p.y(), p.z(), i));
      }
    }
  }
  for (const auto& candidate : candidates) {
    if (checkAc(pinIdx, candidate.pt, candidate.boxIdx)) {
      pin.addAcsPt(candidate.pt);
    }
  }
}

void AcsMgr::computeBoxAcs(const Box<Int_t>& box, const Int_t layerIdx, Vector_t<Point3d<Int_t>>& vAcs) {
  const Int_t lowerGridIdxX = (box.xl() - _cir.gridOffsetX() + _cir.gridStep() - 1) / _cir.gridStep(); // round up 
  const Int_t lowerGridIdxY = (box.yl() - _cir.gridOffsetY() + _cir.gridStep() - 1) / _cir.gridStep(); // round up
  const Int_t higherGridIdxX = (box.xh() - _cir.gridOffsetX()) / _cir.gridStep(); // round down
  const Int_t higherGridIdxY = (box.yh() - _cir.gridOffsetY()) / _cir.gridStep(); // round down
  for (Int_t x = lowerGridIdxX; x <= higherGridIdxX; ++x) {
    for (Int_t y = lowerGridIdxY; y <= higherGridIdxY; ++y) {
      vAcs.emplace_back(x * _cir.gridStep() + _cir.gridOffsetX(),
                        y * _cir.gridStep() + _cir.gridOffsetY(),
                        layerIdx);
    }
  }
}

bool AcsMgr::checkAc(const UInt_t pinIdx, const Point3d<Int_t>& gridPt, const UInt_t boxIdx) const {
  return true;
}

PROJECT_NAMESPACE_END
