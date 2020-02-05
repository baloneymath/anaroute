#include "acsMgr.hpp"

PROJECT_NAMESPACE_START

void AcsMgr::computeAcs()
{
    for (UInt_t pinIdx = 0; pinIdx < _cir.numPins(); ++pinIdx)
    {
        computePinAcs(pinIdx);
    }
}

struct CandidateAcs
{
    public:
        explicit CandidateAcs(Int_t x, Int_t y, Int_t z, UInt_t boxIdx_)
            : pt(x, y, z), boxIdx(boxIdx_) {}
        Point3d<Int_t> pt; ///< The 3D point for representing the grid indices
        UInt_t boxIdx; ///< The index of the box this point is in. pin.box(pt.z(), boxIdx.y())
};

void AcsMgr::computePinAcs(UInt_t pinIdx)
{
    auto & pin = _cir.pin(pinIdx);
    Vector_t<CandidateAcs> candidates; // The candidates for pin access
    for (UInt_t layerIdx = pin.minLayerIdx(); layerIdx <= pin.maxLayerIdx(); ++layerIdx)
    {
        for (UInt_t i = 0; i < pin.numBoxes(layerIdx); ++i)
        {
            const auto & box = pin.box(layerIdx, i);
            Int_t lowerGridIdxX = (box.xl() - _cir.gridOffsetX() + _cir.gridStep() - 1) / _cir.gridStep(); // round up 
            Int_t lowerGridIdxY = (box.yl() - _cir.gridOffsetY() + _cir.gridStep() - 1) / _cir.gridStep(); // round up
            Int_t higherGridIdxX = (box.xh() - _cir.gridOffsetX()) / _cir.gridStep(); // round down
            Int_t higherGridIdxY = (box.yh() - _cir.gridOffsetY()) / _cir.gridStep(); // round down
            for (Int_t x = lowerGridIdxX; x <= higherGridIdxX; ++x)
            {
                for (Int_t y = lowerGridIdxY; y <= higherGridIdxY; ++y)
                {
                    candidates.emplace_back(CandidateAcs(x, y, layerIdx, i));
                }
            }
        }
    }
    for (const auto & candidate : candidates)
    {
       if (checkAc(pinIdx, candidate.pt, candidate.boxIdx))
       {
           pin.addAcsPt(candidate.pt);
       }
    }
}

bool AcsMgr::checkAc(UInt_t pinIdx, Point3d<Int_t> gridPt, UInt_t boxIdx)
{
    return true;
}
PROJECT_NAMESPACE_END
