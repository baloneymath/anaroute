/**
 * @file   dbLef.cpp
 * @brief  Technology configuration from Lef file
 * @author Hao Chen
 * @date   09/12/2019
 *
 **/

#include "dbLef.hpp"

PROJECT_NAMESPACE_START


void LefDB::setVersionStr(const String_t& v) {
  _versionStr = v;
}

void LefDB::setVersion(const Real_t v) {
  _version = v;
}

void LefDB::setDividerChar(const String_t& v) {
  _dividerChar = v;
}

void LefDB::setManufacturingGrid(const Real_t v) {
  _manufacturingGrid = v;
}

void LefDB::setClearanceMeasure(const String_t& v) {
  _clearanceMeasure = v;
}

void LefDB::setBusbitChars(const String_t& v) {
  _busbitChars = v;
}

void LefDB::addRoutingLayer(const LefRoutingLayer& l) {
  _mStr2RoutingLayerIdx[l.name()] = _vRoutingLayers.size();
  _vAllLayers.emplace_back(LayerType_t::ROUTING, _vRoutingLayers.size());
  _vRoutingLayers.emplace_back(l);
}


PROJECT_NAMESPACE_END
