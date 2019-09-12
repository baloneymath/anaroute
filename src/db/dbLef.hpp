/**
 * @file   dbLef.hpp
 * @brief  Technology configuration from Lef file
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#ifndef _DB_TECH_HPP_
#define _DB_TECH_HPP_

#include <limbo/parsers/lef/adapt/LefDataBase.h> // for class lefiXXX

#include "src/global/global.hpp"
#include "lef/lefLayer.hpp"

PROJECT_NAMESPACE_START

class LefDB {
public:
  LefDB() {}
  ~LefDB() {}

private:
  // According to the Cadence Lef API
  std::string   _versionStr;
  std::string   _dividerChar;
  Real_t        _manufacturing;
  std::string   _clearancemeasure;
  lefiUnits     _units;
  std::string   _busbitchars;
  lefiLayer     _layer;
};

PROJECT_NAMESPACE_END

#endif /// _DB_TECH_HPP_
