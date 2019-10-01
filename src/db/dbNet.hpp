/**
 * @file   dbNet.hpp
 * @brief  Circuit Element - Detailed Routing Net
 * @author Hao Chen
 * @date   09/21/2019
 *
 **/

#ifndef _DB_NET_HPP_
#define _DB_NET_HPP_

#include "net/netNode.hpp"

PROJECT_NAMESPACE_START

class Net {
 public:
  Net(const String_t& n = "", const UInt_t idx = MAX_UINT)
    : _name(n), _idx(idx), _symNetIdx(MAX_UINT),
      _bSelfSym(false), _bRouted(false), _failCnt(0) {}
  ~Net() {}

  
  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  const String_t&             name()                    const { return _name; }
  UInt_t                      idx()                     const { return _idx; }
  UInt_t                      symNetIdx()               const { return _symNetIdx; }
  UInt_t                      numPins()                 const { return _vPinIndices.size(); }
  UInt_t                      pinIdx(const UInt_t i)    const { return _vPinIndices[i]; }
  const Vector_t<UInt_t>&     vPinIndices()             const { return _vPinIndices; }
  NetNode&                    node(const UInt_t i)            { return _vNodes[i]; }
  const NetNode&              node(const UInt_t i)      const { return _vNodes[i]; }
  Vector_t<NetNode>&          vNodes()                        { return _vNodes; }
  const Vector_t<NetNode>&    vNodes()                  const { return _vNodes; }
  Int_t                       failCnt()                 const { return _failCnt; }

  bool                        hasSymNet()               const { return _symNetIdx != MAX_UINT; }
  bool                        bSelfSym()                const { return _bSelfSym; }
  bool                        bRouted()                 const { return _bRouted; }

  //////////////////////////////////
  //  Setter                      //
  //////////////////////////////////
  void setName(const String_t& n);
  void setSelfSym();
  void setSymNetIdx(const UInt_t i);
  void setRouted(const bool b = true);
  void addPinIdx(const UInt_t i);
  void addNode(const NetNode& n);
  void addFail();
  void clearFail();

 private:
  String_t            _name;
  UInt_t              _idx;
  UInt_t              _symNetIdx; // MAX_UINT if no sym net
  bool                _bSelfSym;
  bool                _bRouted;
  Vector_t<UInt_t>    _vPinIndices;
  Vector_t<NetNode>   _vNodes;
  Int_t               _failCnt;
};


PROJECT_NAMESPACE_END

#endif /// _DB_NET_HPP_
