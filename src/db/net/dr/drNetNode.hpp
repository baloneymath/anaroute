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
#include "drNetNodeVia.hpp"
#include "src/geo/point3d.hpp"

PROJECT_NAMESPACE_START


class DrNetNode {
 public:
  DrNetNode()
    : _type(DrNetNodeType::MEDIUM), _parent(MAX_INDEX) {}
  ~DrNetNode() {}
  
  // getter
  Point3d<Int_t>&           loc()                         { return _loc; }
  const Point3d<Int_t>&     loc()                   const { return _loc; }
  DrNetNodeType             type()                  const { return _type; }
  DrNetNodeWire             wire()                  const { return _wire; }
  DrNetNodeVia              via()                   const { return _via; }
  Index_t                   parent()                const { return _parent; }
  Index_t                   child(const Index_t i)  const { return _vChilds[i]; }
  Vector_t<Index_t>&        vChilds()                     { return _vChilds; }
  const Vector_t<Index_t>&  vChilds()               const { return _vChilds; }
  
  bool                      hasWire()               const { return _parent != MAX_INDEX; }
  bool                      hasVia()                const { return _via.bValid(); }
  bool                      hasParent()             const { return _parent != MAX_INDEX; }
  bool                      hasChild()              const { return !_vChilds.empty(); }
  
  // setter
  void setLoc(const Point3d<Int_t>& p)                            { _loc = p; }
  void setLoc(const Index_t x, const Index_t y, const Index_t z)  { _loc.setX(x); _loc.setY(y); _loc.setZ(z); }
  void setType(const DrNetNodeType t)                             { _type = t; }
  void setWire(const DrNetNodeWire w)                             { _wire = w; }
  void setVia(const DrNetNodeVia v)                               { _via = v; }
  void setParent(const Index_t i)                                 { _parent = i; }
  void addChild(const Index_t i)                                  { _vChilds.emplace_back(i); }

 private:
  Point3d<Int_t>      _loc;
  DrNetNodeType       _type;
  DrNetNodeWire       _wire;
  DrNetNodeVia        _via;
  Index_t             _parent;
  Vector_t<Index_t>   _vChilds;
  
};

PROJECT_NAMESPACE_END

#endif /// _DB_DR_NET_HPP_
