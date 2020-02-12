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
    _curPinIdx = pinIdx;
    computePinAcs();
  }
}


void AcsMgr::computePinAcs() {
  auto& pin = _cir.pin(_curPinIdx);
  Vector_t<CandidateGridPt> candGridPts; // The candidates for pin access
  for (UInt_t layerIdx = pin.minLayerIdx(); layerIdx <= pin.maxLayerIdx(); ++layerIdx) {
    for (UInt_t i = 0; i < pin.numBoxes(layerIdx); ++i) {
      const auto& box = pin.box(layerIdx, i);
      Vector_t<Point3d<Int_t>> vAcs;
      computeBoxAcs(box, layerIdx, vAcs);
      for (const auto& p : vAcs) {
        candGridPts.emplace_back(CandidateGridPt(p.x(), p.y(), p.z(), i));
      }
    }
  }
  Vector_t<CandidateAcsPt> candAcsPts;
  for (const auto &candGridPt : candGridPts)
  {
    generateCandAcsPt(candGridPt, candAcsPts);
  }
}

void AcsMgr::computeBoxAcs(const Box<Int_t>& box, const Int_t layerIdx, Vector_t<Point3d<Int_t>>& vAcs) {
  const Int_t lowerGridIdxX = (box.xl() - _cir.gridOffsetX() + _cir.gridStep() - 1) / _cir.gridStep(); // round up 
  const Int_t lowerGridIdxY = (box.yl() - _cir.gridOffsetY() + _cir.gridStep() - 1) / _cir.gridStep(); // round up
  const Int_t higherGridIdxX = (box.xh() - _cir.gridOffsetX()) / _cir.gridStep(); // round down
  const Int_t higherGridIdxY = (box.yh() - _cir.gridOffsetY()) / _cir.gridStep(); // round down
  for (Int_t x = lowerGridIdxX; x <= higherGridIdxX; ++x) {
    for (Int_t y = lowerGridIdxY; y <= higherGridIdxY; ++y) {
      vAcs.emplace_back(x, y, layerIdx);
    }
  }
}

void AcsMgr::generateCandAcsPt(const CandidateGridPt &gridPt, Vector_t<CandidateAcsPt> &candAcspts)
{
  const auto &pin = _cir.pin(_curPinIdx);
}

void AcsMgr::generateHorizontalCandAcsPts(const CandidateGridPt &gridPt, Vector_t<CandidateAcsPt> &candAcsPts)
{
  static constexpr Int_t maxAllowedCands = 1;
  UInt_t originCandSize = candAcsPts.size();
}

Box<Int_t> AcsMgr::computeExtensionRect(const CandidateAcsPt &acsPt)
{
  Int_t step = _cir.gridStep(); 
  Int_t width = _cir.lef().routingLayer(_cir.lef().layerPair(acsPt.acs.gridPt().z()).second).minWidth();
  Int_t eolExtension = width;
  Point<Int_t> origin = Point<Int_t>(_cir.gridCenterX(acsPt.acs.gridPt().x()))

}
PROJECT_NAMESPACE_END
