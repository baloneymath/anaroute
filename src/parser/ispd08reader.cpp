/**
 * @file   ispd08reader.cpp
 * @brief  ISPD08-like netlist format parser
 * @author Hao Chen
 * @date   09/22/2019
 *
 **/

#include "ispd08reader.hpp"

PROJECT_NAMESPACE_START

void Ispd08Reader::parse(const String_t& filename) {
  FILE* fin = fopen(filename.c_str(), "r");
  if (!fin) {
    fprintf(stderr, "%s: Error opening file `%s`\n", __func__, filename.c_str());
    exit(0);
  }
  Index_t numNet = 0;
  String_t netName, pinName;
  fscanf(fin, "num net %u\n", &numNet);

  fclose(fin);
}

PROJECT_NAMESPACE_END
