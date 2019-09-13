/**
 * @file   lefLayers.hpp
 * @brief  Technology configuration - Lef layer types
 * @author Hao Chen
 * @date   09/12/2019
 *
 **/

#ifndef _DB_LEF_LAYERTYPE_HPP_
#define _DB_LEF_LAYERTYPE_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START

enum class LayerType_t : Byte_t {
  IMPLANT     = 0,
  MASTERSLICE = 1,
  CUT         = 2,
  ROUTING     = 3,
  OVERLAP     = 4,
  UNKNOWN     = 5
};

PROJECT_NAMESPACE_END

#endif /// _DB_LEF_LAYERTYPE_HPP_
