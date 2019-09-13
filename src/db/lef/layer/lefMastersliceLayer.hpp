/**
 * @file   lefMastersliceLayer.hpp
 * @brief  Technology configuration - Lef Routing layer
 * @author Hao Chen
 * @date   09/12/2019
 *
 **/

#ifndef _DB_LEF_LAYER_MASTERSLICE_HPP_
#define _DB_LEF_LAYER_MASTERSLICE_HPP_

#include "lefLayerBase.hpp"

PROJECT_NAMESPACE_START

class LefMastersliceLayer {
  friend class LefReader;
 
 public:
  LefMastersliceLayer()
    : _name(""), _typeStr("MASTERSLICE"), _type(LayerType_t::MASTERSLICE) {}
  ~LefMastersliceLayer() {}
  
  const String_t&   name()    const { return _name; }
  const String_t&   typeStr() const { return _typeStr; }
  LayerType_t       type()    const { return _type; }

 private:
  String_t      _name;
  String_t      _typeStr;
  LayerType_t   _type;
};


PROJECT_NAMESPACE_END

#endif /// _DB_LEF_LAYER_MASTERSLICE_HPP_
