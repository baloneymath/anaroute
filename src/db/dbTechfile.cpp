/**
 * @file   dbTechfile.cpp
 * @brief  Database - tech file
 * @author Hao Chen
 * @date   09/25/2019
 *
 **/

#include "dbTechfile.hpp"

PROJECT_NAMESPACE_START

void TechfileDB::addStr2LayerMaxIdx(const String_t& n, const Index_t i) {
  assert(_mStr2LayerMaskIdx.count(n) == 0);
  _mStr2LayerMaskIdx[n] = i;
}


PROJECT_NAMESPACE_END
