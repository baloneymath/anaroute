/**
 * @file   dbPin.cpp
 * @brief  Circuit Element - Pin
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#include "dbPin.hpp"

PROJECT_NAMESPACE_START

void Pin::setName(const String_t& n) {
  _name = n;
}

void Pin::setNetName(const String_t& n) {
  _netName = n;
}

void Pin::setNetIdx(const UInt_t i) {
  _netIdx = i;
}

void Pin::setLayerIdxOffset(const UInt_t i ) {
  _layerIdxOffset = i;
}

void Pin::addBox(const UInt_t layerIdx, const Box<Int_t>& box) {
  UInt_t idx = layerIdx - _layerIdxOffset;
  if (idx >= _vvBoxes.size()) {
    _vvBoxes.resize(idx + 1);
  }
  _vvBoxes[idx].emplace_back(box);
}

void Pin::setLayerBoxes(const UInt_t layerIdx, const Vector_t<Box<Int_t>>& vBoxes) {
  UInt_t idx = layerIdx - _layerIdxOffset;
  if (idx >= _vvBoxes.size()) {
    _vvBoxes.resize(idx + 1);
  }
  _vvBoxes[idx] = vBoxes;
}

void Pin::printInfo() const {

}

PROJECT_NAMESPACE_END
