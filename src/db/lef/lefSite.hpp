/**
 * @file   lefSite.hpp
 * @brief  Technology configuration - Lef Site
 * @author Hao Chen
 * @date   09/15/2019
 *
 **/

#ifndef _DB_LEF_SITE_HPP_
#define _DB_LEF_SITE_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START

enum class LefSiteClassType_t {
  PAD  = 0,
  CORE = 1
};

enum class LefSiteSymmetricType_t {
  X   = 0,
  Y   = 1,
  R90 = 2
};

enum class LefSiteOrient_t {
  N  = 0,
  S  = 1,
  E  = 2,
  W  = 3,
  FN = 4,
  FS = 5,
  FE = 6,
  FW = 7
};

class LefSiteRowPattern {
 public:
  Vector_t<Pair_t<String_t, LefSiteOrient_t>> vSites; // first: previous site name
                                                      // second: previous site orient
};

class LefSite {
  friend class LefReader;
 public:
  LefSite() {}
  ~LefSite() {}

 private:
  String_t                _name;
  String_t                _classTypeStr;
  LefSiteClassType_t      _classType;
  String_t                _symmetricTypeStr;
  LefSiteSymmetricType_t  _symmetricType;
       //_vRowPatterns;
  Int_t                   _sizeX;
  Int_t                   _sizeY;

};

PROJECT_NAMESPACE_END

#endif /// _DB_LEF_SITE_HPP_
