/**
 * @file   dbCir.cpp
 * @brief  The Top-Level Database - Circuit
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#include "dbCir.hpp"

PROJECT_NAMESPACE_START

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
  _vPins.emplace_back(p);
}

void CirDB::addNet(const Net& n) {
  _mStr2NetIdx[n.name()] = _vNets.size();
  _vNets.emplace_back(n);
}

void CirDB::addBlock(const UInt_t i, const Block& b) {
  _vvBlocks[i].emplace_back(b);
}
void CirDB::resizeVVBlocks(const UInt_t i) {
  _vvBlocks.resize(i);
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
        fprintf(fout, "      %d (%d %d %d %d)\n", pin.minLayerIdx() + j,
                                                  pin.box(pin.minLayerIdx() + j, k).xl(),
                                                  pin.box(pin.minLayerIdx() + j, k).yl(),
                                                  pin.box(pin.minLayerIdx() + j, k).xh(),
                                                  pin.box(pin.minLayerIdx() + j, k).yh());
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
  for (const auto& vBlocks : _vvBlocks) {
    for (const auto& block : vBlocks) {
      fprintf(fout, "    BLOCK %u (%d %d %d %d)\n", block.layerIdx(),
                                                    block.xl(),
                                                    block.yl(),
                                                    block.xh(),
                                                    block.yh());
    }
  }
}

PROJECT_NAMESPACE_END
