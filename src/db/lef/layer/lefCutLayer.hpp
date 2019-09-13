/**
 * @file   lefCutLayer.hpp
 * @brief  Technology configuration - Lef Routing layer
 * @author Hao Chen
 * @date   09/12/2019
 *
 **/


#ifndef _DB_LEF_LAYER_CUT_HPP_
#define _DB_LEF_LAYER_CUT_HPP_

#include "lefLayerBase.hpp"

PROJECT_NAMESPACE_START

class LefCutLayer {
  friend class LefReader;
 
 public:
  LefCutLayer()
    : _name(""), _typeStr("CUT"), _type(LayerType_t::CUT) {}
  ~LefCutLayer() {}

 private:
  String_t _name;
  String_t _typeStr;
  LayerType_t _type;

};

PROJECT_NAMESPACE_END

#endif /// _DB_LEF_LAYER_CUT_HPP_
