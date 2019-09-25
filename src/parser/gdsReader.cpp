/**
 * @file   gdsReader.cpp
 * @brief  Parser - Gds format parser
 * @author Hao Chen
 * @date   09/24/2019
 *
 **/

#include "gdsReader.hpp"
#include "src/geo/polygon2box.hpp"

using Limbo_GdsDB = GdsParser::GdsDB::GdsDB;
using Limbo_GdsReader = GdsParser::GdsDB::GdsReader;

PROJECT_NAMESPACE_START

void GdsReader::parse(const String_t& filename) {
  // Flatten Gds by the last cell
  Limbo_GdsDB unflatenDB;
  Limbo_GdsReader limboReader(unflatenDB);
  if (!limboReader(filename)) {
    fprintf(stderr, "%s: Cannot open file %s!!\n", __func__, filename.c_str());
    exit(0);
  }
  // set unit scale
  _scale = std::round(1e-6 / unflatenDB.precision());
  // Flatten gds
  String_t topCellName = topCell(unflatenDB);
  auto flatCell = unflatenDB.extractCell(topCellName);
  GdsParser::GdsDB::GdsDB gdsDB;
  gdsDB.addCell(flatCell);
  //Build the mapping between the gds layer and the router layers
  buildLayerMap();
}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////
String_t GdsReader::topCell(GdsParser::GdsDB::GdsDB db) {
  // Whether each cell is found as the subcell of the other
  UMap_t<String_t, bool> mNameFound;
  // Iterate all the cells and record their names
  // Add reversely
  for (Int_t i = db.cells().size() - 1; i >= 0; --i) {
    mNameFound[db.cells().at(i).name()] = false;
  }
  for (const auto& cell : db.cells()) {
    for (const auto& obj : cell.objects()) {
      String_t name = "";
      GdsParser::GdsDB::GdsObjectHelpers()(obj.first, obj.second, GetSRefNameActionParser(name));
      if (name != "") {
        mNameFound[name] = true;
      }
    }
  }
  // Return the name that was not included in the other cells
  for (const auto& pair : mNameFound) {
    if (pair.first != "" and pair.second == false) {
      return pair.first;
    }
  }
  return "";
}

void GdsReader::buildLayerMap() {
  for (Index_t layerIdx = 0; layerIdx < _cir.lef().numLayers(); ++layerIdx) {
    if (_cir.lef().bRoutingLayer(layerIdx)) {
    }
  }
}

/////////////////////////////////////////
//    Helper functions                 //
/////////////////////////////////////////
Int_t GdsReader::to_db_unit(const Int_t n) const {
  assert(_scale != 0);
  return n * _scale;
}


PROJECT_NAMESPACE_END
