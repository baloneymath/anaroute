/**
 * @file   drNet.hpp
 * @brief  Circuit Element - Detailed Routing Net
 * @author Hao Chen
 * @date   09/21/2019
 *
 **/

#ifndef _DB_DR_NET_HPP_
#define _DB_DR_NET_HPP_

#include "net/netNode.hpp"

PROJECT_NAMESPACE_START

class Net {
 public:
  Net(const String_t& n = "", const Index_t idx = MAX_INDEX)
    : _name(n), _idx(idx), _symNetIdx(MAX_INDEX), _bSelfSym(false) {}
  ~Net() {}

  
  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  const String_t&             name()                    const { return _name; }
  Index_t                     idx()                     const { return _idx; }
  Index_t                     symNetIdx()               const { return _symNetIdx; }
  Index_t                     numPins()                 const { return _vPinIndices.size(); }
  Index_t                     pinIdx(const Index_t i)   const { return _vPinIndices[i]; }
  const Vector_t<Index_t>&    vPinIndices()             const { return _vPinIndices; }
  NetNode&                    node(const Index_t i)           { return _vNodes[i]; }
  const NetNode&              node(const Index_t i)     const { return _vNodes[i]; }
  Vector_t<NetNode>&          vNodes()                        { return _vNodes; }
  const Vector_t<NetNode>&    vNodes()                  const { return _vNodes; }
  
  bool                        hasSymNet()               const { return _symNetIdx != MAX_INDEX; }
  bool                        bSelfSym()                const { return _bSelfSym; }

  //////////////////////////////////
  //  Setter                      //
  //////////////////////////////////
  void setName(const String_t& n);
  void setSelfSym();
  void setSymnetIdx(const Index_t i);
  void addPinIdx(const Index_t i);
  void addNode(const NetNode& n);

 private:
  String_t            _name;
  Index_t             _idx;
  Index_t             _symNetIdx; // MAX_INDEX if no sym net
  bool                _bSelfSym;
  Vector_t<Index_t>   _vPinIndices;
  Vector_t<NetNode>   _vNodes; // Maintain in topological order, _vNodes[0] is the driver
};


PROJECT_NAMESPACE_END

#endif /// _DB_DR_NET_HPP_
