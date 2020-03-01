/**
 * @file   drSymmetry.cpp
 * @brief  Degree of symmetric detection
 * @author Hao Chen
 * @date   02/26/2020
 *
 **/

#include "drSymmetry.hpp"
#include "src/graph/bipartite.hpp"
#include "src/graph/matching.hpp"

PROJECT_NAMESPACE_START

void DrSymmetry::solve() {
  for (UInt_t i = 0; i < _cir.numNets(); ++i) {
    Net& net = _cir.net(i);
    net.setSymAxisX(_cir.symAxisX());
  }
}

void DrSymmetry::bestMatching(const Net& net1, const Net& net2, Int_t& bestSymAxisX, Float_t& maxDegSymPre) {
  auto accumX = [&] (const Int_t pinIdx) -> Pair_t<Int_t, Int_t> {
    const Pin& pin = _cir.pin(pinIdx);
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Int_t sumX = 0, boxCnt = 0;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        sumX += cpBox->centerX();
        boxCnt += 1;
      }
    }
    return std::make_pair(sumX, boxCnt);
  };
  maxDegSymPre = 0;
  for (const Int_t pinIdx1 : net1.vPinIndices()) {
    const auto pair1 = accumX(pinIdx1);
    for (const Int_t pinIdx2 : net2.vPinIndices()) {
      const auto pair2 = accumX(pinIdx2);
      const Int_t symAxisX = (pair1.first + pair2.first) / (pair1.second + pair2.second);
      Float_t degSym = degSymPre(net1, net2, symAxisX);
      if (degSym > maxDegSymPre) {
        maxDegSymPre = degSym;
        bestSymAxisX = symAxisX;
      }
    }
  }
}

Float_t DrSymmetry::degSymPre(const Net& net1, const Net& net2, const Int_t symAxisX) {
  if (&net1 == &net2) {
    return degSelfSymPre(net1, symAxisX);
  }
  // construct pin locations vector
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes;
  addPinShapes(net2, vvBoxes);

  // count match pins
  Int_t match = 0;
  for (const Int_t pinIdx : net1.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bExistTotallySymPin(pin, vvBoxes))
      ++match;
  }
  const Int_t numPins = std::max(net1.numPins(), net2.numPins());
  Float_t ratio = (Float_t)match / numPins;
  return ratio;
}

Float_t DrSymmetry::degSelfSymPre(const Net& net, const Int_t symAxisX) {
  // construct pin locations vector
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes;
  addPinShapes(net, vvBoxes);

  // count match pins
  Int_t match = 0;
  for (const Int_t pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bExistTotallySymPin(pin, vvBoxes))
      ++match;
  }
  Float_t ratio = (Float_t)match / net.numPins();
  return ratio;
}

void DrSymmetry::addPinShapes(const Net& net, Vector_t<Vector_t<Box<Int_t>>>& vvBoxes) {
  vvBoxes.resize(_cir.lef().numLayers());
  for (const Int_t pinIdx : net.vPinIndices()) {
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vvBoxes[layerIdx].emplace_back(*cpBox);
      }
    }
  }
  for (auto& vBoxes : vvBoxes) {
    std::sort(vBoxes.begin(), vBoxes.end());
  }
}

bool DrSymmetry::bExistTotallySymPin(const Pin& pin, const Vector_t<Vector_t<Box<Int_t>>>& vvSymBoxes) {
  const Net& net = _cir.net(pin.netIdx());
  UInt_t i, layerIdx;
  const Box<Int_t>* cpBox;
  Pin_ForEachLayerIdx(pin, layerIdx) {
    Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
      Box<Int_t> symBox(*cpBox);
      symBox.flipX(net.symAxisX());
      if (!std::binary_search(vvSymBoxes[layerIdx].begin(), vvSymBoxes[layerIdx].end(), symBox)) {
        return false;
      }
    }
  }
  return true;
}


PROJECT_NAMESPACE_END
