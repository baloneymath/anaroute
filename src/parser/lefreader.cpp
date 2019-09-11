/**
 * @file   lefreader.cpp
 * @brief  Lef file parser using Limbo wrapper
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#include "lefreader.hpp"

using namespace std;

PROJECT_NAMESPACE_START

void LefReader::lef_version_cbk(std::string const& v) {}
void LefReader::lef_version_cbk(double v) {}
void LefReader::lef_dividerchar_cbk(const std::string &v) {}
void LefReader::lef_casesensitive_cbk(int v) {}
void LefReader::lef_nowireextension_cbk(const std::string &v) {}
void LefReader::lef_manufacturing_cbk(double v) {}
void LefReader::lef_useminspacing_cbk(lefiUseMinSpacing const &v) {}
void LefReader::lef_clearancemeasure_cbk(const std::string &v) {}
void LefReader::lef_units_cbk(lefiUnits const &v) {}
void LefReader::lef_busbitchars_cbk(const std::string &v) {}
void LefReader::lef_layer_cbk(lefiLayer const &v) {}
void LefReader::lef_maxstackvia_cbk(lefiMaxStackVia const &v) {}
void LefReader::lef_via_cbk(lefiVia const &v) {}
void LefReader::lef_viarule_cbk(lefiViaRule const &v) {}
void LefReader::lef_spacing_cbk(lefiSpacing const &v) {}
void LefReader::lef_irdrop_cbk(lefiIRDrop const &v) {}
void LefReader::lef_minfeature_cbk(lefiMinFeature const &v) {}
void LefReader::lef_dielectric_cbk(double v) {}
void LefReader::lef_nondefault_cbk(lefiNonDefault const &v) {}
void LefReader::lef_site_cbk(lefiSite const &v) {}
void LefReader::lef_macrobegin_cbk(std::string const &v) {}
void LefReader::lef_macro_cbk(lefiMacro const &v) {}
void LefReader::lef_pin_cbk(lefiPin const &v) {}
void LefReader::lef_obstruction_cbk(lefiObstruction const &v) {}
void LefReader::lef_density_cbk(lefiDensity const &v) {}
void LefReader::lef_timing_cbk(lefiTiming const &v) {}
void LefReader::lef_array_cbk(lefiArray const &v) {}
void LefReader::lef_prop_cbk(lefiProp const &v) {}
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
void LefReader::lef_extension_cbk(const std::string &v) {}


PROJECT_NAMESPACE_END


