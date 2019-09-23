/**
 * @file   drNetNodeVia.hpp
 * @brief  Net Element - Node via
 * @author Hao Chen
 * @date   09/22/2019
 *
 **/

#ifndef _DB_DR_NET_NODE_VIA_HPP_
#define _DB_DR_NET_NODE_VIA_HPP_

#include "drNetBase.hpp"

PROJECT_NAMESPACE_START

class DrNetNodeVia {
 public:
  DrNetNodeVia()
    : _viaType(MAX_INDEX) {}
  ~DrNetNodeVia() {}

  // getter
  bool      bValid()    const { return _viaType != MAX_INDEX; }
  Index_t   viaType()   const { return _viaType; }

  // setter
  void setViaType(const Index_t i) { _viaType = i; }
 private:
  Index_t   _viaType; // The index of via in LefDB

};

PROJECT_NAMESPACE_END

#endif /// _DB_DR_NET_NODE_VIA_HPP_
