/**
 * @file   drSymmetry.cpp
 * @brief  Degree of symmetric detection
 * @author Hao Chen
 * @date   02/26/2020
 *
 **/

#include "drSymmetry.hpp"

PROJECT_NAMESPACE_START

void DrSymmetry::solve() {
  for (UInt_t i = 0; i < _cir.numNets(); ++i) {
    Net& net = _cir.net(i);
    net.setSymAxisX(_cir.symAxisX());
  }
}

Float_t DrSymmetry::computeDegreeOfSym(const Net& net1, const Net& net2) {
  if (&net1 == &net2) {
    computeDegreeOfSelfSym(net1);
    return 1;
  }
}

Float_t DrSymmetry::computeDegreeOfSelfSym(const Net& net) {
  
}


PROJECT_NAMESPACE_END
