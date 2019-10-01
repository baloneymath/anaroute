/**
 * @file   parIspd08.cpp
 * @brief  ISPD08-like netlist format parser
 * @author Hao Chen
 * @date   09/22/2019
 *
 **/

#include "parIspd08.hpp"
#include "src/util/util.hpp"
#include "src/geo/point.hpp"
#include "src/geo/box.hpp"
#include "src/geo/polygon2box.hpp"

PROJECT_NAMESPACE_START

void Ispd08Reader::parse(const String_t& filename) {
  assert(_cir.lef().version() > 0);
  FILE* fin = fopen(filename.c_str(), "r");
  if (!fin) {
    fprintf(stderr, "%s: Error opening file `%s`\n", __func__, filename.c_str());
    exit(0);
  }
  
  setScale();
  buildLayerMap();

  constexpr UInt_t bufSize = 200;
  char buf[bufSize];

  UInt_t numNet = 0;
  fscanf(fin, "num net %u\n", &numNet);
  for (UInt_t i = 0; i < numNet; ++i) {
    char netName[bufSize];
    UInt_t netIdx = 0, numPolygon = 0;
    Int_t minWidth = 0;
    fscanf(fin, "%s %d %d %d\n", netName, &netIdx, &numPolygon, &minWidth);
    Net net(netName, netIdx);
    for (UInt_t j = 0; j < numPolygon; ++j) {
      Vector_t<String_t> vTokens;
      fgets(buf, bufSize, fin);
      util::splitString(buf, " ", vTokens);
      Pin pin;
      Int_t layerIdx = map2RoutingLayer(std::stoi(vTokens[0]));
      pin.setLayerIdxOffset(layerIdx);
      Vector_t<Point<Int_t>> vPts;
      for (UInt_t k = 1; k + 1 < vTokens.size(); k += 2) {
        Int_t x = to_db_unit(std::stoi(vTokens[k]));
        Int_t y = to_db_unit(std::stoi(vTokens[k + 1]));
        vPts.emplace_back(x, y);
      }
      Vector_t<Box<Int_t>> vBoxes;
      if (!geo::polygon2Box<Int_t>(vPts, vBoxes)) {
        fprintf(stderr, "%s: Error polygon format!!!\n", __func__);
        exit(0);
      }
      pin.setLayerBoxes(layerIdx, vBoxes);
      net.addPinIdx(_cir.numPins());
      _cir.addPin(pin);
    }
    _cir.addNet(net);
  }
  fclose(fin);
}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////
void Ispd08Reader::setScale() {
  assert(_cir.lef().units().databaseNumber() >= 1000 and _cir.lef().units().databaseNumber() % 1000 == 0);
  _scale = _cir.lef().units().databaseNumber() / 1000;
}

void Ispd08Reader::buildLayerMap() {
  for (UInt_t i = 0; i < _cir.lef().numLayers(); ++i) {
    if (_cir.lef().bRoutingLayer(i)) {
      _vIdx2RoutingLayerIdx.emplace_back(i);
    }
  }
}

UInt_t Ispd08Reader::map2RoutingLayer(const UInt_t i) {
  assert(i > 0);
  return _vIdx2RoutingLayerIdx[i - 1];
}
/////////////////////////////////////////
//    Helper functions                 //
/////////////////////////////////////////
Int_t Ispd08Reader::to_db_unit(const Int_t n) const {
  return n * _scale;
}

PROJECT_NAMESPACE_END
