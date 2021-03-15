/**
 * @file   parGds.cpp
 * @brief  Parser - Gds format parser
 * @author Hao Chen
 * @date   09/24/2019
 *
 **/

#include "parGds.hpp"
#include "src/geo/polygon2box.hpp"
#include "src/geo/box2polygon.hpp"

PROJECT_NAMESPACE_START

void GdsReader::parse(const String_t& filename) {
  // Flatten Gds by the last cell
  GdsParser::GdsDB::GdsDB unflatenDB;
  GdsParser::GdsDB::GdsReader limboReader(unflatenDB);
  if (!limboReader(filename)) {
    fprintf(stderr, "GdsReader::%s ERROR: Cannot open file %s!!\n", __func__, filename.c_str());
    exit(0);
  }
  
  
  // Flatten gds
  String_t topCellName = topCell(unflatenDB);
  auto flatCell = unflatenDB.extractCell(topCellName);
  GdsParser::GdsDB::GdsDB gdsDB;
  gdsDB.addCell(flatCell);
  
  // Build the mapping between the gds layer and the router layers
  buildLayerMap();
  
  // Process the shapes in gds
  for (const auto& obj : flatCell.objects()) {
    GdsParser::GdsDB::GdsObjectHelpers()(obj.first, obj.second, ExtractShapeLayerAction(_vMaskId2Layers, _vPolygonLayers));
  }
	// Read in poblk
  for (const auto& obj : flatCell.objects()) {
    GdsParser::GdsDB::GdsObjectHelpers()(obj.first, obj.second, ExtractPoBlkAction(_vMaskId2Layers, _vPoblks));
  }
	
  // scale the design
  const Int_t gds_unit = std::round(1e-6 / unflatenDB.precision());
  const Int_t db_unit = _cir.lef().units().databaseNumber();
  const Float_t sc = db_unit / gds_unit;

  for (auto& polygon : _vPolygonLayers) {
    polygon.scale(sc, sc);
  }
  for (auto& poblk : _vPoblks) {
    poblk.scale(sc, sc);
  }
  // save to db
  saveShapesAsBlockages();
  // Parse OD layer
  parseOdLayers(flatCell, sc);
}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////
String_t GdsReader::topCell(const GdsParser::GdsDB::GdsDB& db) {
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
  for (UInt_t layerIdx = 0; layerIdx < _cir.lef().numLayers(); ++layerIdx) {
    if (_cir.lef().bRoutingLayer(layerIdx)) {
      _vMaskId2Layers.emplace_back(_cir.layerIdx2MaskIdx(layerIdx), layerIdx);
    }
  }
}

void GdsReader::saveShapesAsBlockages() {
  // Get poblks
  Vector_t<Box<Int_t>> vBlks;
  for (const auto& blk : _vPoblks) {
    geo::polygon2Box(blk.pts, vBlks);
  }
  _cir.resizeVVBlkIndices(_cir.lef().numLayers());
  Vector_t<Box<Int_t>> vBoxes;
  UInt_t cnt = 0;
  for (const auto& poly : _vPolygonLayers) {
    geo::polygon2Box(poly.pts, vBoxes);
    for (const auto& box : vBoxes) {
      auto __addBlk = [&] (const UInt_t idx, const UInt_t layerIdx, const auto& box, const bool bLVS = false) {
        Blk b(idx, layerIdx, box, bLVS);
        _cir.addBlk(layerIdx, b);
        if (b.xl() < _cir.xl()) {
          _cir.setXL(b.xl());
        }
        if (b.yl() < _cir.yl()) {
          _cir.setYL(b.yl());
        }
        if (b.xh() > _cir.xh()) {
          _cir.setXH(b.xh());
        }
        if (b.yh() > _cir.yh()) {
          _cir.setYH(b.yh());
        }
      };
      if (poly.layer == MAX_UINT) {
        // dummy blks
        //for (const auto& blk : vBlks) {
          //if (Box<Int_t>::bCover(blk, box)) {
            //for (UInt_t layerIdx = 0; layerIdx < _cir.lef().numLayers(); ++layerIdx) {
            for (UInt_t layerIdx = 0; layerIdx <= _cir.lef().routingLayerIdx2LayerIdx(4); ++layerIdx) {
              if (_cir.lef().bRoutingLayer(layerIdx)) {
                Box<Int_t> lvsBox1(box.xl(), box.yl() + 100, box.xh(), box.centerY());
                Box<Int_t> lvsBox2(box.xl(), box.centerY(), box.xh(), box.yh() - 100);
                __addBlk(cnt++, layerIdx, lvsBox1, true);
                __addBlk(cnt++, layerIdx, lvsBox2, true);
              }
            //}
          //}
        }
      }
      else {
        __addBlk(cnt++, poly.layer, box);
      }
    }
    vBoxes.clear();
  }
}

void GdsReader::parseOdLayers(GdsParser::GdsDB::GdsCell &flatCell, Float_t sc)
{
  // Parse in the od layer
  Int_t odLayer = _cir.tech().mStr2LayerMaskIdx().at("OD");
  Vector_t<PolygonLayer> odPolygons;
  for (const auto &obj : flatCell.objects())
  {
      GdsParser::GdsDB::GdsObjectHelpers()(obj.first, obj.second, ExtractSpecificShapeLayerAction(odLayer, odPolygons));
  }
  // Convert into rectangles
  Vector_t<Box<Int_t>> rects;
  for (auto& polyLayer : odPolygons)
  {
      polyLayer.scale(sc, sc);
      geo::polygon2Box(polyLayer.pts, rects);
  }
  // Eliminate the overlapping parts
  geo::boxes2BoxesEmplace(rects);
  for (const auto &rect : rects)
  {
      _cir.addSpatialOD(rect);
  }
}

PROJECT_NAMESPACE_END
