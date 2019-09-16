/**
 * @file   lefVia.cpp
 * @brief  Technology configuration - Lef Via
 * @author Hao Chen
 * @date   09/16/2019
 *
 **/

#include "lefVia.hpp"

PROJECT_NAMESPACE_START

void LefVia::setName(const String_t& n) {
  _name = n;
}

void LefVia::setDefault() {
  _bDefault = true;
}

void LefVia::setBotLayerIdx(const Index_t i) {
  _botLayerIdx = i;
}

void LefVia::setCutLayerIdx(const Index_t i) {
  _cutLayerIdx = i;
}

void LefVia::setTopLayerIdx(const Index_t i) {
  _topLayerIdx = i;
}

void LefVia::setBotLayerName(const String_t& n) {
  _botLayerName = n;
}

void LefVia::setCutLayerName(const String_t& n) {
  _cutLayerName = n;
}

void LefVia::setTopLayerName(const String_t& n) {
  _topLayerName = n;
}

void LefVia::addBotBox(const Box<Int_t>& b) {
  _vBotBoxes.emplace_back(b);
}

void LefVia::addCutBox(const Box<Int_t>& b) {
  _vCutBoxes.emplace_back(b);
}

void LefVia::addTopBox(const Box<Int_t>& b) {
  _vTopBoxes.emplace_back(b);
}

PROJECT_NAMESPACE_END

