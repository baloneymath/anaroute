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
#include "src/geo/box.hpp"

PROJECT_NAMESPACE_START

class Routable {
 public:
  Routable(const bool bSelfSym = false,
           const Int_t idx = -1,
           const Int_t netIdx = -1,
           const Int_t symNetIdx = -1,
           const Int_t symNetRoutableIdx = -1)
    : _bSelfSym(bSelfSym),
      _idx(idx),
      _netIdx(netIdx),
      _symNetIdx(symNetIdx),
      _symNetRoutableIdx(symNetRoutableIdx),
      _bRouted(false) {}
  ~Routable() {}

  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  Vector_t<Int_t>& vPinIndices()      { return _vPinIndices; }
  Vector_t<Int_t>& vRoutableIndices() { return _vRoutableIndices; }
  Vector_t<Int_t>& vWireIndices()     { return _vWireIndices; }
  Vector_t<Int_t>& vPathIndices()     { return _vPathIndices; }
  const Vector_t<Int_t>& vPinIndices()      const { return _vPinIndices; }
  const Vector_t<Int_t>& vRoutableIndices() const { return _vRoutableIndices; }
  const Vector_t<Int_t>& vWireIndices()     const { return _vWireIndices; }
  const Vector_t<Int_t>& vPathIndices()     const { return _vPathIndices; }
  
  Int_t   numPins()                   const { return _vPinIndices.size(); }
  Int_t   numRoutables()              const { return _vRoutableIndices.size(); }
  Int_t   pinIdx(const Int_t i)       const { return _vPinIndices[i]; }
  Int_t   routableIdx(const Int_t i)  const { return _vRoutableIndices[i]; }

  bool    bSelfSym()          const { return _bSelfSym; }
  bool    hasSymNet()         const { return _symNetIdx != -1; }
  Int_t   idx()               const { return _idx; }
  Int_t   netIdx()            const { return _netIdx; }
  Int_t   symNetIdx()         const { return _symNetIdx; }
  Int_t   symNetRoutableIdx() const { return _symNetRoutableIdx; }
  
  bool    bRouted()           const { return _bRouted; }

  //////////////////////////////////
  //  Setter                      //
  //////////////////////////////////
  void setSelfSym(const bool b = true) { _bSelfSym = b; }
  void setIdx(const Int_t i) { _idx = i; }
  void setNetIdx(const Int_t i) { _netIdx = i; }
  void setSymNetIdx(const Int_t i) { _symNetIdx = i; }
  void setSymNetRoutableIdx(const Int_t i) { _symNetRoutableIdx = i; }
  void addPinIdx(const Int_t i) { _vPinIndices.emplace_back(i); }
  void addRoutableIdx(const Int_t i) { _vRoutableIndices.emplace_back(i); }
  void addWireIdx(const Int_t i) { _vWireIndices.emplace_back(i); }
  void addPathIdx(const Int_t i) { _vPathIndices.emplace_back(i); }

  void setRouted(const bool b = true) { _bRouted = b; }

 private:
  Vector_t<Int_t> _vPinIndices;
  Vector_t<Int_t> _vRoutableIndices; // idx of the routables of the net
  Vector_t<Int_t> _vWireIndices; // idx of _vWires in Net
  Vector_t<Int_t> _vPathIndices; // idx of _vRoutePaths in Net

  bool    _bSelfSym;
  Int_t   _idx;
  Int_t   _netIdx;
  Int_t   _symNetIdx;
  Int_t   _symNetRoutableIdx;

  bool    _bRouted;

};

class Net {
 public:
  Net(const String_t& n = "", const UInt_t idx = MAX_UINT)
    : _name(n), _idx(idx), _symNetIdx(MAX_UINT),
      _bSelfSym(false), _bRouted(false), _bIOPort(false),
      _grFailCnt(0), _drFailCnt(0) {}
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
  UInt_t                      numNodes()                const { return _vNodes.size(); }
  NetNode&                    node(const UInt_t i)            { return _vNodes[i]; }
  const NetNode&              node(const UInt_t i)      const { return _vNodes[i]; }
  Vector_t<NetNode>&          vNodes()                        { return _vNodes; }
  const Vector_t<NetNode>&    vNodes()                  const { return _vNodes; }
  bool                        hasSymNet()               const { return _symNetIdx != MAX_UINT; }
  bool                        bSelfSym()                const { return _bSelfSym; }
  bool                        bIOPort()                 const { return _bIOPort; }
  
  // global routing
  Int_t                                       grFailCnt()    const { return _grFailCnt; }
  UInt_t                                      numGuides()    const { return _vGuides.size(); }
  Vector_t<Pair_t<Box<Int_t>, Int_t>>&        vGuides()            { return _vGuides; }
  const Vector_t<Pair_t<Box<Int_t>, Int_t>>&  vGuides()      const { return _vGuides; }
  // detailed routing
  Int_t                                                   drFailCnt()   const { return _drFailCnt; }
  bool                                                    bRouted()     const { return _bRouted; }
  Vector_t<Pair_t<Box<Int_t>, Int_t>>&                    vWires()            { return _vWires; }
  const Vector_t<Pair_t<Box<Int_t>, Int_t>>&              vWires()      const { return _vWires; }
  Vector_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>&       vRoutePaths()       { return _vRoutePaths; } 
  const Vector_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>& vRoutePaths() const { return _vRoutePaths; } 

  // net ordering
  Box<Int_t>         bbox()  const { return Box<Int_t>(_bboxXL, _bboxYL, _bboxXH, _bboxYH); }

  // for performance driven and electrical consideration
  bool    bPower()        const { return _bPower; }
  Int_t   minWidth()      const { return _minWidth; }
  Int_t   minCuts()       const { return _minCuts; }
  Int_t   numCutsRow()    const { return _numCutsRow; }
  Int_t   numCutsCol()    const { return _numCutsCol; }

  // for not-quite-symmetric structure
  Int_t                       numRoutables()                  const { return _vRoutables.size(); }
  Int_t                       routableSchedule(const Int_t i) const { return _vRoutableSchedule[i]; }
  Routable&                   routable(const Int_t i)               { return _vRoutables[i]; }
  Vector_t<Routable>&         vRoutables()                          { return _vRoutables; }
  Vector_t<Int_t>&            vRoutableSchedule()                   { return _vRoutableSchedule; }
  const Routable&             routable(const Int_t i)         const { return _vRoutables[i]; }
  const Vector_t<Routable>&   vRoutables()                    const { return _vRoutables; }
  const Vector_t<Int_t>&      vRoutableSchedule()             const { return _vRoutableSchedule; }

  Int_t                       symAxisX()                      const { return _symAxisX; }

  // evaluation
  Int_t                       viaCnt()      const { return _viaCnt; }
  Int_t                       wireLength()  const { return _wireLength; }
  Float_t                     wireArea()    const { return _wireArea; }
  Float_t                     degSym()      const { return _degSym; }
  //////////////////////////////////
  //  Setter                      //
  //////////////////////////////////
  void setName(const String_t& n);
  void setSelfSym();
  void setIOPort();
  void setSymNetIdx(const UInt_t i);
  void setRouted(const bool b = true);
  void addPinIdx(const UInt_t i);
  void addNode(const NetNode& n);
  void addGrFail();
  void addDrFail();
  void clearGrFail();
  void clearDrFail();
  void setGuides(const Vector_t<Pair_t<Box<Int_t>, Int_t>>& v);

  void resetBBox();
  void updateBBox(const Box<Int_t>& box);

  void setPower(const bool b = true);
  void setMinWidth(const Int_t w);
  void setMinCuts(const Int_t c);
  void setNumCutsRow(const Int_t c);
  void setNumCutsCol(const Int_t c);

  void clearRoutables();
  void clearRouting();

  void setSymAxisX(const Int_t x);

  void setViaCnt(const Int_t n);
  void setWireLength(const Int_t l);
  void setWireArea(const Float_t a);
  void setDegSym(const Float_t d);
  
  void computeNSetWireLength();
  void computeNSetWireArea();

 private:
  String_t                                          _name;
  UInt_t                                            _idx;
  UInt_t                                            _symNetIdx; // MAX_UINT if no sym net
  bool                                              _bSelfSym;
  bool                                              _bRouted;
  bool                                              _bIOPort;
  Vector_t<UInt_t>                                  _vPinIndices;
  Vector_t<NetNode>                                 _vNodes;
  Int_t                                             _grFailCnt;
  Int_t                                             _drFailCnt;
  Vector_t<Pair_t<Box<Int_t>, Int_t>>               _vGuides; // from global routing
  Vector_t<Pair_t<Box<Int_t>, Int_t>>               _vWires;
  Vector_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>  _vRoutePaths;

  // for net ordering
  Int_t _bboxXL = MAX_INT; // bounding box of pin locations
  Int_t _bboxYL = MAX_INT; // bounding box of pin locations
  Int_t _bboxXH = MIN_INT; // bounding box of pin locations
  Int_t _bboxYH = MIN_INT; // bounding box of pin locations

  // for performance driven and electrical consideration
  bool    _bPower = false;
  Int_t   _minWidth = 0;
  Int_t   _minCuts = 1;
  Int_t   _numCutsRow = 1;
  Int_t   _numCutsCol = 1;

  // for not-quite-symmetric structure
  Vector_t<Routable>  _vRoutables;
  Vector_t<Int_t>     _vRoutableSchedule;
  Int_t               _symAxisX;

  // evaluation
  Int_t   _viaCnt = 0;
  Int_t   _wireLength = 0;
  Float_t _wireArea = 0;
  Float_t _degSym = 0;
};

//////////////////////////////////
//  Iterators                   //
//////////////////////////////////
#define Net_ForEachPinIdx(net, idx, i) \
  for (i = 0, idx = net.pinIdx(i); i < net.numPins(); ++i, idx = net.pinIdx(i))

#define Net_ForEachNetNode(net, pNode_, i) \
  for (i = 0; i < net.numNodes() and (pNode_ = &net.node(i)); ++i)

#define Net_ForEachNetNodeC(net, cpNode_, i) \
  for (i = 0; i < net.numNodes() and (cpNode_ = &net.node(i)); ++i)

#define Net_ForEachRoutedWire(net, pWire_, i) \
  for (i = 0; i < net.vWires().size() and (pWire_ = &net.vWires()[i]); ++i)

PROJECT_NAMESPACE_END

#endif /// _DB_NET_HPP_
