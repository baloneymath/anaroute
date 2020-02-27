/**
 * @file   drSymmetry.hpp
 * @brief  Degree of symmetric detection
 * @author Hao Chen
 * @date   02/26/2020
 *
 **/

#ifndef _DR_SYMMETRY_HPP_
#define _DR_SYMMETRY_HPP_

#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class DrSymmetry {
 public:
  DrSymmetry(CirDB& cir)
    : _cir(cir) {}
  ~DrSymmetry() {}

  void solve();

  Float_t computeDegreeOfSym(const Net& net1, const Net& net2);
  Float_t computeDegreeOfSelfSym(const Net& net);

 private:
  CirDB& _cir;
};

PROJECT_NAMESPACE_END

#endif /// _DR_SYMMETRY_HPP_
