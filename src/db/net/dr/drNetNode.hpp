/**
 * @file   drNetNode.hpp
 * @brief  Net Element - Node
 * @author Hao Chen
 * @date   09/21/2019
 *
 **/

#ifndef _DB_DR_NET_NODE_HPP_
#define _DB_DR_NET_NODE_HPP_

#include "drNetBase.hpp"
#include "drNetNodeWire.hpp"
#include "src/geo/point3d.hpp"

PROJECT_NAMESPACE_START


class DrNetNode {
 public:
  DrNetNode()
    : _hasWire(false) {}
  ~DrNetNode() {}
  
  // getter
  bool hasWire() const { return _hasWire; }

  // setter
  void setHasWire() { _hasWire = true; }

 private:
  Point3d<Int_t> _loc;
  bool _hasWire; // true if has wire (this->parent, this)
  DrNetNodeType _type;
  DrNetNodeWire _wire;
   
};

PROJECT_NAMESPACE_END

#endif /// _DB_DR_NET_HPP_
