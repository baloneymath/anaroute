/**
 * @file   lefRoutingLayer.cpp
 * @brief  Technology configuration - Lef Routing layer
 * @author Hao Chen
 * @date   09/12/2019
 *
 **/

#include "lefRoutingLayer.hpp"

PROJECT_NAMESPACE_START

void LefRoutingLayer::setName(const String_t& n) {
  _name = n;
}

void LefRoutingLayer::setType(const String_t& t) {
  _typeStr = t;
  if (t == "IMPLANT") _type = LayerType_t::IMPLANT;
  else if (t == "MASTERSLICE") _type = LayerType_t::MASTERSLICE;
  else if (t == "CUT") _type = LayerType_t::CUT;
  else if (t == "ROUTING") _type = LayerType_t::ROUTING;
  else if (t == "OVERLAP") _type = LayerType_t::OVERLAP;
  else assert(false);
}

void LefRoutingLayer::setRouteDir(const String_t& d) {
  _routeDirStr = d;
  if (d == "HORIZONTAL") _routeDir = RouteDir_t::HORIZONTAL;
  else if (d == "VERTICAL") _routeDir = RouteDir_t::VERTICAL;
  else assert(false);
}

void LefRoutingLayer::setMinArea(const Int_t a) {
  _minArea = a;
}

void LefRoutingLayer::setMinWidth(const Int_t w) {
  _minWidth = w;
}

void LefRoutingLayer::setMaxWidth(const Int_t w) {
  _maxWidth = w;
}

void LefRoutingLayer::setDefaultWidth(const Int_t w) {
  _defaultWidth = w;
}

void LefRoutingLayer::setPitch(const Int_t p) {
  _pitch = p;
}

void LefRoutingLayer::setPitchX(const Int_t p) {
  _pitchX = p;
}

void LefRoutingLayer::setPitchY(const Int_t p) {
  _pitchY = p;
}

void LefRoutingLayer::setOffset(const Int_t o) {
  _offset = o;
}

void LefRoutingLayer::setOffsetX(const Int_t o) {
  _offsetX = o;
}

void LefRoutingLayer::setOffsetY(const Int_t o) {
  _offsetY = o;
}

void LefRoutingLayer::addSpacing(const Int_t s) {
  _vSpacings.emplace_back(s);
}

void LefRoutingLayer::addEolSpacing(const Int_t s, const Int_t d, const Int_t w) {
  _vEolSpacings.emplace_back(s, d, w);
}

void LefRoutingLayer::addParallelRunLength(const Int_t p) {
  _spacingTable.vParallelRunLength.emplace_back(p);
}

void LefRoutingLayer::addSpacingTableWidth(const Int_t w) {
  _spacingTable.table.emplace_back(w, Vector_t<Int_t>());
}

void LefRoutingLayer::addSpacingTableWidthSpacing(const Index_t idx, const Int_t s) {
  _spacingTable.table[idx].second.emplace_back(s);
}

void LefRoutingLayer::logInfo() const {
  printf("%s\n", _name.c_str());
  printf("%s\n", _typeStr.c_str());
}

PROJECT_NAMESPACE_END
