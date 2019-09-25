/**
 * @file   techfileReader.cpp
 * @brief  Parser - Tsmc techfile (techlayer only)
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#include "techfileReader.hpp"

PROJECT_NAMESPACE_START

void TechfileReader::parse(const String_t& filename) {
  FILE* fin = fopen(filename.c_str(), "r");
  if (!fin) {
    fprintf(stderr, "%s: Error opening file %s!!!\n", __func__, filename.c_str());
    exit(0);
  }
  constexpr Index_t bufSize = 200;
  char buf[bufSize];

  fclose(fin);
}

void TechfileReader::readTechLayers() {

}

PROJECT_NAMESPACE_END
