/**
 * @file   drNet.hpp
 * @brief  Circuit Element - Detailed Routing Net
 * @author Hao Chen
 * @date   09/21/2019
 *
 **/

#ifndef _DB_DR_NET_HPP_
#define _DB_DR_NET_HPP_

#include "drNetNode.hpp"

PROJECT_NAMESPACE_START

class DrNet {
 public:
  DrNet() {}
  ~DrNet() {}

 private:
  String_t _name;
  Index_t  _symNetId; // MAX_INDEX if no sym net
  Vector_t<Pin> _vPins;
  Vector_t<DrNetNode> _vNodes;
};


PROJECT_NAMESPACE_END

#endif /// _DB_DR_NET_HPP_
