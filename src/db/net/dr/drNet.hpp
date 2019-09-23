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
  DrNet()
    : _name(""), _symNetIdx(MAX_INDEX) {}
  ~DrNet() {}

  
  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  const String_t&             name()                    const { return _name; }
  Index_t                     symNetIdx()               const { return _symNetIdx; }
  Index_t                     pinIdx(const Index_t i)   const { return _vPinIndices[i]; }
  const Vector_t<Index_t>&    vPinIndices()             const { return _vPinIndices; }
  DrNetNode&                  node(const Index_t i)           { return _vNodes[i]; }
  const DrNetNode&            node(const Index_t i)     const { return _vNodes[i]; }
  Vector_t<DrNetNode>&        vNodes()                        { return _vNodes; }
  const Vector_t<DrNetNode>&  vNodes()                  const { return _vNodes; }
  
  bool                        hasSymnet()               const { return _symNetIdx != MAX_INDEX; }

  //////////////////////////////////
  //  Setter                      //
  //////////////////////////////////
  void setName(const String_t& n);
  void setSymnetIdx(const Index_t i);
  void addPinIdx(const Index_t i);
  void addNode(const DrNetNode& n);

 private:
  String_t            _name;
  Index_t             _symNetIdx; // MAX_INDEX if no sym net
  Vector_t<Index_t>   _vPinIndices;
  Vector_t<DrNetNode> _vNodes; // Maintain in topological order, _vNodes[0] is the driver
};


PROJECT_NAMESPACE_END

#endif /// _DB_DR_NET_HPP_
