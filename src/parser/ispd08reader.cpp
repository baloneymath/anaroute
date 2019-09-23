/**
 * @file   ispd08reader.cpp
 * @brief  ISPD08-like netlist format parser
 * @author Hao Chen
 * @date   09/22/2019
 *
 **/

#include "ispd08reader.hpp"
#include "src/util/util.hpp"

PROJECT_NAMESPACE_START

void Ispd08Reader::parse(const String_t& filename) {
  FILE* fin = fopen(filename.c_str(), "r");
  if (!fin) {
    fprintf(stderr, "%s: Error opening file `%s`\n", __func__, filename.c_str());
    exit(0);
  }
  
  constexpr Index_t bufSize = 200;
  char buf[bufSize];

  Index_t numNet = 0;
  fscanf(fin, "num net %u\n", &numNet);
  for (Index_t i = 0; i < numNet; ++i) {
    char netName[bufSize];
    Index_t netIdx = 0, numPolygon = 0;
    Int_t minWidth = 0;
    fscanf(fin, "%s %d %d %d\n", netName, &netIdx, &numPolygon, &minWidth);
    printf("%s %d %d %d\n", netName, netIdx, numPolygon, minWidth);
    for (Index_t j = 0; j < numPolygon; ++j) {
      Vector_t<String_t> vTokens;
      fgets(buf, bufSize, fin);
      util::splitString(buf, " ", vTokens);
      Int_t layerIdx = std::stoi(vTokens[0]);
      printf("%d ", layerIdx);
      for (Index_t k = 1; k < vTokens.size(); k += 2) {
        Int_t x = std::stoi(vTokens[k]);
        Int_t y = std::stoi(vTokens[k + 1]);
        printf("%d %d ", x, y);
      }
      printf("\n");
    }
  }
  fclose(fin);
}

PROJECT_NAMESPACE_END
