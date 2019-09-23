/**
 * @file   drNet.cpp
 * @brief  Circuit Element - Detailed Routing Net
 * @author Hao Chen
 * @date   09/21/2019
 *
 **/

#include "drNet.hpp"

PROJECT_NAMESPACE_START

void DrNet::setName(const String_t& n) {
  _name = n;
}

void DrNet::setSymnetIdx(const Index_t i) {
  _symNetIdx = i;
}

void DrNet::addPinIdx(const Index_t i) {
  _vPinIndices.emplace_back(i);
}

void DrNet::addNode(const DrNetNode& n) {
  // TODO
}

PROJECT_NAMESPACE_END
