/**
 * @file   dbCir.cpp
 * @brief  The Top-Level Database - Circuit
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#include "dbCir.hpp"

PROJECT_NAMESPACE_START

void CirDB::addPin(const Pin& p) {
  _vPins.emplace_back(p);
}

void CirDB::addDrNet(const DrNet& n) {
  _vDrNets.emplace_back(n);
}

void CirDB::setStr2LayerMaxIdx() {
}

void CirDB::printInfo() const {
  FILE* fout = stderr;
  fprintf(fout, "CIRCUIT %s\n", _name.c_str());
  fprintf(fout, "  NUM PINS %lu\n", _vPins.size());
  for (Index_t i = 0; i < _vPins.size(); ++i) {
    fprintf(fout, "    PIN %s\n", _vPins[i].name().c_str());
    const auto& vvBoxes = _vPins[i].vvBoxes();
    for (Index_t j = 0; j < vvBoxes.size(); ++j) {
      for (Index_t k = 0; k < vvBoxes[j].size(); ++k) {
        fprintf(fout, "      %d (%d %d %d %d)\n", _vPins[i].minLayerIdx() + j,
                                                  _vPins[i].box(_vPins[i].minLayerIdx() + j, k).xl(),
                                                  _vPins[i].box(_vPins[i].minLayerIdx() + j, k).yl(),
                                                  _vPins[i].box(_vPins[i].minLayerIdx() + j, k).xh(),
                                                  _vPins[i].box(_vPins[i].minLayerIdx() + j, k).yh());
      }
    }
  }
  fprintf(fout, "\n  NUM NETS %lu\n", _vDrNets.size());
  for (Index_t i = 0; i < _vDrNets.size(); ++i) {
    fprintf(fout, "    NET %s\n", _vDrNets[i].name().c_str());
    for (Index_t j = 0; j < _vDrNets[i].numPins(); ++j) {
      fprintf(fout, "      PIN %u\n", _vDrNets[i].pinIdx(j));
    }
  }
}

PROJECT_NAMESPACE_END
