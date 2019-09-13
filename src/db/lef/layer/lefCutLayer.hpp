/**
 * @file   lefCutLayer.hpp
 * @brief  Technology configuration - Lef Routing layer
 * @author Hao Chen
 * @date   09/12/2019
 *
 **/


#ifndef _DB_LEF_LAYER_CUT_HPP_
#define _DB_LEF_LAYER_CUT_HPP_

#include "lefLayerType.hpp"

PROJECT_NAMESPACE_START

class LefCutLayer {
  friend class LefReader;
 
 public:
  LefCutLayer() {}
  ~LefCutLayer() {}

 private:
};

PROJECT_NAMESPACE_END

#endif /// _DB_LEF_LAYER_CUT_HPP_
