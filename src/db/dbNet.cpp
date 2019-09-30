/**
 * @file   drNet.cpp
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

void Net::setSymnetIdx(const Index_t i) {
  _symNetIdx = i;
}

void Net::addPinIdx(const Index_t i) {
  _vPinIndices.emplace_back(i);
}

void Net::addNode(const NetNode& n) {
  // TODO
}

PROJECT_NAMESPACE_END
