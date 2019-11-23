/**
 * @file   writer.cpp
 * @brief  Output file writer
 * @author Hao Chen
 * @date   11/21/2019
 *
 **/

#include "writer.hpp"

PROJECT_NAMESPACE_START

// result
void Writer::writeLayoutGds(const String_t& placementFilename, const String_t& outputFilename) {
  _layoutw.writeGds(placementFilename, outputFilename);
}

// guide
void Writer::writeGrGuide(const String_t& filename) const {
  _grGuidew.write(filename);
}

void Writer::writeGrGuideGds(const String_t& netName, const String_t& filename) const {
  _grGuidew.writeGds(netName, filename);
}

PROJECT_NAMESPACE_END
