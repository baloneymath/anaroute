/**
 * @file   lefreader.cpp
 * @brief  Lef file parser using Limbo wrapper
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#include "lefreader.hpp"

using std::round;

PROJECT_NAMESPACE_START

void LefReader::parse(const String_t& filename) {
  LefParser::read(*this, filename.c_str()); // Limbo defined function
}

void LefReader::lef_version_cbk(String_t const& v) {
  _lef.setVersionStr(v);
}

void LefReader::lef_version_cbk(double v) {
  _lef.setVersion(v);
}

void LefReader::lef_dividerchar_cbk(const String_t &v) {
  _lef.setDividerChar(v);
}

void LefReader::lef_casesensitive_cbk(int v) {}

void LefReader::lef_nowireextension_cbk(const String_t &v) {}

void LefReader::lef_manufacturing_cbk(double v) {
  _lef.setManufacturingGrid(v);
}

void LefReader::lef_useminspacing_cbk(lefiUseMinSpacing const &v) {}

void LefReader::lef_clearancemeasure_cbk(const String_t &v) {
  _lef.setClearanceMeasure(v);
}

void LefReader::lef_units_cbk(lefiUnits const &v) {
  LefUnits& units = _lef.units();
  if (v.hasDatabase()) {
    units.setDatabaseName(v.databaseName());
    units.setDatabaseNumber(round(v.databaseNumber()));
  }
  if (v.hasCapacitance()) {
    units.setCapacitance(round(v.capacitance()));
  }
  if (v.hasResistance()) {
    units.setResistance(round(v.resistance()));
  }
  if (v.hasTime()) {
    units.setTime(round(v.time()));
  }
  if (v.hasPower()) {
    units.setPower(round(v.power()));
  }
  if (v.hasCurrent()) {
    units.setCurrent(round(v.current()));
  }
  if (v.hasVoltage()) {
    units.setVoltage(round(v.voltage()));
  }
  if (v.hasFrequency()) {
    units.setFrequency(round(v.frequency()));
  }
}

void LefReader::lef_busbitchars_cbk(const String_t &v) {
  _lef.setBusbitChars(v);
}

void LefReader::lef_layer_cbk(lefiLayer const &v) {
  assert(_lef.units().databaseNumber() != 0);
  if (v.hasType()) {
    if (strcmp(v.type(), "IMPLANT") == 0) {
      parseImplantLayer(v);
    }
    else if (strcmp(v.type(), "MASTERSLICE") == 0) {
      parseMastersliceLayer(v);
    }
    else if (strcmp(v.type(), "CUT") == 0) {
      parseCutLayer(v);
    }
    else if (strcmp(v.type(), "ROUTING") == 0) {
      parseRoutingLayer(v);
    }
    else if (strcmp(v.type(), "OVERLAP") == 0) {
      parseOverlapLayer(v);
    }
    else
      assert(false);
  }
  else
    assert(false);
}

void LefReader::lef_maxstackvia_cbk(lefiMaxStackVia const &v) {}
void LefReader::lef_via_cbk(lefiVia const &v) {}
void LefReader::lef_viarule_cbk(lefiViaRule const &v) {}
void LefReader::lef_spacing_cbk(lefiSpacing const &v) {}
void LefReader::lef_irdrop_cbk(lefiIRDrop const &v) {}
void LefReader::lef_minfeature_cbk(lefiMinFeature const &v) {}
void LefReader::lef_dielectric_cbk(double v) {}
void LefReader::lef_nondefault_cbk(lefiNonDefault const &v) {}

void LefReader::lef_site_cbk(lefiSite const &v) {
  LefSite site;
  // name
  site.setName(v.name());
  if (v.hasClass()) {
    site.setClassType(v.siteClass());
  }
  if (v.hasXSymmetry()) {
    site.setXSymmetry();
  }
  if (v.hasYSymmetry()) {
    site.setYSymmetry();
  }
  if (v.has90Symmetry()) {
    site.setR90Symmetry();
  }
  if (v.hasRowPattern()) {
    for (Index_t i = 0; i < (Index_t)v.numSites(); ++i) {
      site.addRowPattern(v.siteName(i), v.siteOrientStr(i));
    }
  }
  site.setSizeX(v.sizeX());
  site.setSizeY(v.sizeY());
}

void LefReader::lef_macrobegin_cbk(String_t const &v) {}
void LefReader::lef_macro_cbk(lefiMacro const &v) {}
void LefReader::lef_pin_cbk(lefiPin const &v) {}
void LefReader::lef_obstruction_cbk(lefiObstruction const &v) {}
void LefReader::lef_density_cbk(lefiDensity const &v) {}
void LefReader::lef_timing_cbk(lefiTiming const &v) {}
void LefReader::lef_array_cbk(lefiArray const &v) {}
void LefReader::lef_prop_cbk(lefiProp const &v) {

}
void LefReader::lef_noisemargin_cbk(lefiNoiseMargin const &v) {}
void LefReader::lef_edgeratethreshold1_cbk(double v) {}
void LefReader::lef_edgeratethreshold2_cbk(double v) {}
void LefReader::lef_edgeratescalefactor_cbk(double v) {}
void LefReader::lef_noisetable_cbk(lefiNoiseTable const &v) {}
void LefReader::lef_correctiontable_cbk(lefiCorrectionTable const &v) {}
void LefReader::lef_inputantenna_cbk(double v) {}
void LefReader::lef_outputantenna_cbk(double v) {}
void LefReader::lef_inoutantenna_cbk(double v) {}
void LefReader::lef_antennainput_cbk(double v) {}
void LefReader::lef_antennaoutput_cbk(double v) {}
void LefReader::lef_antennainout_cbk(double v) {}
void LefReader::lef_extension_cbk(const String_t &v) {}

/////////////////// Private Functions ////////////////////////
void LefReader::parseImplantLayer(const lefiLayer& v) {
  LefImplantLayer layer;
  // name
  layer.setName(v.name());
  _lef.addImplantLayer(layer);
}

void LefReader::parseMastersliceLayer(const lefiLayer& v) {
  LefMastersliceLayer layer;
  // name
  layer.setName(v.name());
  _lef.addMastersliceLayer(layer);
}

void LefReader::parseCutLayer(const lefiLayer& v) {
  LefCutLayer layer;
  // name
  layer.setName(v.name());
  if (v.hasWidth()) {
    layer.setMinWidth(to_lef_unit_1d(v.width()));
  }
  layer.setMinWidth(v.width());
  for (Index_t i = 0; i < (Index_t)v.numSpacing(); ++i) {
    if (v.hasSpacingAdjacent(i)) {
      /// TODO
    }
    else if (v.hasSpacingParallelOverlap(i)) {
      layer.setParallelOverlapSpacing(to_lef_unit_1d(v.spacing(i)));
    }
    else if (v.hasSpacingSamenet(i)) {
      layer.setSameNetSpacing(to_lef_unit_1d(v.spacing(i)));
    }
    else {
      layer.setSpacing(to_lef_unit_1d(v.spacing(i)));
    }
  }
  _lef.addCutLayer(layer);
}

void LefReader::parseRoutingLayer(const lefiLayer& v) {
  LefRoutingLayer layer;
  // name
  layer.setName(v.name());
  // direction
  if (v.hasDirection()) {
    layer.setRouteDir(v.direction());
  }
  // area
  if (v.hasArea()) {
    layer.setMinArea(to_lef_unit_2d(v.area()));
  }
  // width
  if (v.hasWidth()) {
    Int_t width = to_lef_unit_1d(v.width());
    layer.setDefaultWidth(width);
    layer.setMinWidth(width);
  }
  if (v.hasMinwidth()) {
    layer.setMinWidth(to_lef_unit_1d(v.minwidth()));
  }
  if (v.hasMaxwidth()) {
    layer.setMaxWidth(to_lef_unit_1d(v.maxwidth()));
  }
  // spacing
  for (Index_t i = 0; i < (Index_t)v.numSpacing(); ++i) {
    if (v.hasSpacingEndOfLine(i)) {
      layer.addEolSpacing(to_lef_unit_1d(v.spacing(i)),
                          to_lef_unit_1d(v.spacingEolWidth(i)),
                          to_lef_unit_1d(v.spacingEolWithin(i)));
    }
    else {
      layer.addSpacing(to_lef_unit_1d(v.spacing(i)));
    }
  }
  // spacing table
  for (Index_t i = 0; i < (Index_t)const_cast<lefiLayer&>(v).numSpacingTable(); ++i) {
    const lefiSpacingTable* spTable = const_cast<lefiLayer&>(v).spacingTable(i);
    if (spTable->isParallel()) {
      const lefiParallel* parallel = spTable->parallel();
      for (Index_t j = 0; j < (Index_t)parallel->numLength(); ++j) {
        layer.addParallelRunLength(to_lef_unit_1d(parallel->length(j)));
      }
      for (Index_t j = 0; j < (Index_t)parallel->numWidth(); ++j) {
        layer.addSpacingTableWidth(to_lef_unit_1d(parallel->width(j)));
        for (Index_t k = 0; k < (Index_t)parallel->numLength(); ++k) {
          layer.addSpacingTableWidthSpacing(j, to_lef_unit_1d(parallel->widthSpacing(j, k)));
        }
      }
    }
  }
  // pitch
  if (v.hasPitch()) {
    layer.setPitch(to_lef_unit_1d(v.pitch()));
  }
  if (v.hasXYPitch()) {
    layer.setPitchX(to_lef_unit_1d(v.pitchX()));
    layer.setPitchY(to_lef_unit_1d(v.pitchY()));
  }
  // offset
  if (v.hasOffset()) {
    layer.setOffset(to_lef_unit_1d(v.offset()));
  }
  if (v.hasXYOffset()) {
    layer.setOffsetX(to_lef_unit_1d(v.offsetX()));
    layer.setOffsetY(to_lef_unit_1d(v.offsetY()));
  }
  // add to LefDB
  _lef.addRoutingLayer(layer);
}

void LefReader::parseOverlapLayer(const lefiLayer& v) {
  LefOverlapLayer layer;
  // name
  layer.setName(v.name());
  _lef.addOverlapLayer(layer);
}

////////////// Helper Functions //////////////////////////////
Int_t LefReader::to_lef_unit_1d(const Real_t n) const {
  assert(_lef.units().hasDatabase());
  Int_t unitLEF = _lef.units().databaseNumber();
  return std::round(n * unitLEF);
}
Int_t LefReader::to_lef_unit_2d(const Real_t n) const {
  assert(_lef.units().hasDatabase());
  Int_t unitLEF = _lef.units().databaseNumber();
  return std::round(n * unitLEF * unitLEF);
}

PROJECT_NAMESPACE_END


