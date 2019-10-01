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

void Net::addFail() {
  ++_failCnt;
}
void Net::clearFail() {
  _failCnt = 0;
}

PROJECT_NAMESPACE_END
