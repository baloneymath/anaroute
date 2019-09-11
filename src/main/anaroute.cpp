/**
 * @file   anaroute.cpp
 * @brief  Analog Router
 * @author Hao Chen
 * @date   09/01/2019
 *
 **/

#include "anaroute.hpp"
#include "include/cmdline.h"
#include "src/parser/lefreader.hpp"

using namespace std;

PROJECT_NAMESPACE_START

Anaroute::Anaroute(int argc, char** argv) {
  parseArgs(argc, argv);
  LefReader lefr;
}

void Anaroute::parseArgs(int argc, char** argv) {
  _args.add<string>("tech_lef", '\0', "technology lef file");
  _args.add<string>("tech_file", '\0', "technology layer file");
  _args.add<string>("design_type", '\0', "design type");
  _args.add<string>("design_file", '\0', "design file");
  _args.add<string>("placement_layout", '\0', "placement layout file");
  _args.add<string>("iopin", '\0', "IO-Pin file");
  _args.add<string>("symnet", '\0', "symmetric nets file");
  _args.add<string>("out", 'o', "output file");
  _args.parse_check(argc, argv);
}


PROJECT_NAMESPACE_END


