/**
 * @file   dbNet.cpp
 * @brief  Circuit Element - Detailed Routing Net
 * @author Hao Chen
 * @date   09/21/2019
 *
 **/

#include "dbNet.hpp"

PROJECT_NAMESPACE_START

void Net::setName(const String_t& n) {
  _name = n;
}

void Net::setSelfSym() {
  _bSelfSym = true;
}

void Net::setIOPort() {
  _bIOPort = true;
}

void Net::setSymNetIdx(const UInt_t i) {
  _symNetIdx = i;
}

void Net::setRouted(const bool b) {
  _bRouted = b;
}

void Net::addPinIdx(const UInt_t i) {
  _vPinIndices.emplace_back(i);
}

void Net::addNode(const NetNode& n) {
  // TODO
  _vNodes.emplace_back(n);
}

void Net::addGrFail() {
  ++_grFailCnt;
}

void Net::addDrFail() {
  ++_drFailCnt;
}

void Net::clearGrFail() {
  _grFailCnt = 0;
}

void Net::clearDrFail() {
  _drFailCnt = 0;
}

void Net::setGuides(const Vector_t<Pair_t<Box<Int_t>, Int_t>>& v) {
  _vGuides = v;
}

void Net::resetBBox() {
  _bboxXL = MAX_INT;
  _bboxYL = MAX_INT;
  _bboxXH = MIN_INT;
  _bboxYH = MIN_INT;
}

void Net::updateBBox(const Box<Int_t>& box) {
  _bboxXL = std::min(_bboxXL, box.xl());
  _bboxYL = std::min(_bboxYL, box.yl());
  _bboxXH = std::max(_bboxXH, box.xh());
  _bboxYH = std::max(_bboxYH, box.yh());
}

void Net::setPowerGround(const bool b) {
  _bPowerGround = b;
}
 
void Net::setMinWidth(const Int_t w) {
  _minWidth = w;
}

void Net::setMinCuts(const Int_t c) {
  _minCuts = c;
}

void Net::clearRoutables() {
  _vRoutables.clear();
  _vRoutableSchedule.clear();
}

void Net::clearRouting() {
  for (auto& ro : _vRoutables) {
    ro.vWireIndices().clear();
    ro.setRouted(false);
  }
  _vWires.clear();
  _bRouted = false;
}

void Net::setSymAxisX(const Int_t x) {
  _symAxisX = x;
}

PROJECT_NAMESPACE_END
