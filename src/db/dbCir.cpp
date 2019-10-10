/**
 * @file   dbCir.cpp
 * @brief  The Top-Level Database - Circuit
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#include "dbCir.hpp"

PROJECT_NAMESPACE_START

//////////////////////////////////
//  Funcs                       //
//////////////////////////////////
void CirDB::buildSpatial() {
  buildSpatialPins();
  buildSpatialBlks();
}

void CirDB::buildSpatialPins() {
  Vector_t<Vector_t<spatial::b_value<Int_t, UInt_t>>> vvShapes;
  vvShapes.resize(_lef.numLayers());
  UInt_t i, j, layerIdx;
  const Pin* cpPin;
  const Box<Int_t>* cpBox;
  Cir_ForEachPinC((*this), cpPin, i) {
    Pin_ForEachLayerIdx((*cpPin), layerIdx) {
      Pin_ForEachLayerBoxC((*cpPin), layerIdx, cpBox, j) {
        vvShapes[layerIdx].emplace_back(spatial::b_box<Int_t>(cpBox->bl(), cpBox->tr()), i);
      }
    }
  }
  _vSpatialPins.resize(_lef.numLayers());
  Cir_ForEachLayerIdx((*this), layerIdx) {
    _vSpatialPins[layerIdx] = SpatialMap<Int_t, UInt_t>(vvShapes[layerIdx]);
  }
}

void CirDB::buildSpatialBlks() {
  Vector_t<Vector_t<spatial::b_value<Int_t, UInt_t>>> vvShapes;
  vvShapes.resize(_lef.numLayers());
  _vSpatialBlks.resize(_lef.numLayers());
  UInt_t i, layerIdx;
  const Blk* cpBlk;
  Cir_ForEachLayerIdx((*this), layerIdx) {
    Cir_ForEachLayerBlkC((*this), layerIdx, cpBlk, i) {
      vvShapes[layerIdx].emplace_back(spatial::b_box<Int_t>(cpBlk->bl(), cpBlk->tr()), i);
    }
    _vSpatialBlks[layerIdx] = SpatialMap<Int_t, UInt_t>(vvShapes[layerIdx]);
  }
}

bool CirDB::queryPin(const UInt_t layerIdx, const Point<Int_t>& bl, const Point<Int_t>& tr, Vector_t<UInt_t>& vPinIndices) {
  assert(layerIdx >= 0 and layerIdx < _vSpatialPins.size());
  _vSpatialPins[layerIdx].query(bl, tr, vPinIndices);
  return vPinIndices.size() > 0;
}

bool CirDB::queryPin(const UInt_t layerIdx, const Box<Int_t>& box, Vector_t<UInt_t>& vPinIndices) {
  assert(layerIdx >= 0 and layerIdx < _vSpatialPins.size());
  _vSpatialPins[layerIdx].query(box, vPinIndices);
  return vPinIndices.size() > 0;
}

bool CirDB::queryBlk(const UInt_t layerIdx, const Point<Int_t>& bl, const Point<Int_t>& tr, Vector_t<UInt_t>& vBlkIndices) {
  assert(layerIdx >= 0 and layerIdx < _vSpatialBlks.size());
  _vSpatialBlks[layerIdx].query(bl, tr, vBlkIndices);
  return vBlkIndices.size() > 0;
}

bool CirDB::queryBlk(const UInt_t layerIdx, const Box<Int_t>& box, Vector_t<UInt_t>& vBlkIndices) {
  assert(layerIdx >= 0 and layerIdx < _vSpatialBlks.size());
  _vSpatialBlks[layerIdx].query(box, vBlkIndices);
  return vBlkIndices.size() > 0;
}

//////////////////////////////////
//  Private Setter              //
//////////////////////////////////
void CirDB::setXL(const Int_t x) {
  _xl = x;
}

void CirDB::setYL(const Int_t y) {
  _yl = y;
}

void CirDB::setXH(const Int_t x) {
  _xh = x;
}

void CirDB::setYH(const Int_t y) {
  _yh = y;
}

void CirDB::addPin(const Pin& p) {
  assert(_vvPinIndices.size() > 0);
  UInt_t layerIdx;
  Pin_ForEachLayerIdx(p, layerIdx) {
    _vvPinIndices[layerIdx].emplace_back(_vPins.size());
  }
  _vPins.emplace_back(p);
}

void CirDB::addNet(const Net& n) {
  _mStr2NetIdx[n.name()] = _vNets.size();
  _vNets.emplace_back(n);
}

void CirDB::addBlk(const UInt_t i, const Blk& b) {
  assert(_vvBlkIndices.size() > 0);
  _vvBlkIndices[i].emplace_back(_vBlks.size());
  _vBlks.emplace_back(b);
}

void CirDB::resizeVVPinIndices(const UInt_t i) {
  _vvPinIndices.resize(i);
}

void CirDB::resizeVVBlkIndices(const UInt_t i) {
  _vvBlkIndices.resize(i);
}

UInt_t CirDB::layerIdx2MaskIdx(const UInt_t i) const {
  const Pair_t<LefLayerType, UInt_t>& p = _lef.layerPair(i);
  switch (p.first) {
    case LefLayerType::IMPLANT:     return _tech.str2LayerMaskIdx(_lef.implantLayer(p.second).name());
    case LefLayerType::MASTERSLICE: return _tech.str2LayerMaskIdx(_lef.mastersliceLayer(p.second).name());
    case LefLayerType::CUT:         return _tech.str2LayerMaskIdx(_lef.cutLayer(p.second).name());
    case LefLayerType::ROUTING:     return _tech.str2LayerMaskIdx(_lef.routingLayer(p.second).name());
    case LefLayerType::OVERLAP:     return _tech.str2LayerMaskIdx(_lef.overlapLayer(p.second).name());
    default: assert(false);
  }
  return MAX_UINT;
}

// for debug
void CirDB::printInfo() const {
  FILE* fout = stdout;
  fprintf(fout, "CIRCUIT %s (%d %d %d %d)\n", _name.c_str(), _xl, _yl, _xh, _yh);
  fprintf(fout, "  NUM PINS %lu\n", _vPins.size());
  for (UInt_t i = 0; i < _vPins.size(); ++i) {
    const Pin& pin = _vPins[i];
    fprintf(fout, "    PIN %s\n", pin.name().c_str());
    const auto& vvBoxes = pin.vvBoxes();
    for (UInt_t j = 0; j < vvBoxes.size(); ++j) {
      for (UInt_t k = 0; k < vvBoxes[j].size(); ++k) {
        fprintf(fout, "      %d (%d %d %d %d)\n", j,
                                                  pin.box(j, k).xl(),
                                                  pin.box(j, k).yl(),
                                                  pin.box(j, k).xh(),
                                                  pin.box(j, k).yh());
      }
    }
  }
  fprintf(fout, "\n  NUM NETS %lu\n", _vNets.size());
  for (UInt_t i = 0; i < _vNets.size(); ++i) {
    const Net& net = _vNets[i];
    fprintf(fout, "    NET %s", net.name().c_str());
    if (net.bSelfSym())
      fprintf(fout, " SELFSYM");
    if (net.hasSymNet())
      fprintf(fout, " SYMNET %s", _vNets[net.symNetIdx()].name().c_str());
    fprintf(fout, "\n");
    for (UInt_t j = 0; j < net.numPins(); ++j) {
      fprintf(fout, "      PIN %u\n", net.pinIdx(j));
    }
  }
  fprintf(fout, "\n  TSMC TECHLAYER %lu\n", _tech.mStr2LayerMaskIdx().size());
  for (const auto& obj : _tech.mStr2LayerMaskIdx()) {
    fprintf(fout, "    LAYER %s %d\n", obj.first.c_str(), obj.second);
  }
  fprintf(fout, "\n  BLOCKS\n");
  for (const auto& blk : _vBlks) {
    fprintf(fout, "    BLOCK %u (%d %d %d %d)\n", blk.layerIdx(),
                                                  blk.xl(),
                                                  blk.yl(),
                                                  blk.xh(),
                                                  blk.yh());
  }
}

PROJECT_NAMESPACE_END
