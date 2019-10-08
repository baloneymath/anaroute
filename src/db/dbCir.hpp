/**
 * @file   dbCir.hpp
 * @brief  The Top-Level Database - Circuit
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#ifndef _DB_CIRCUIT_HPP_
#define _DB_CIRCUIT_HPP_

#include "src/global/global.hpp"
#include "dbLef.hpp"
#include "dbTechfile.hpp"
#include "dbBlock.hpp"
#include "dbPin.hpp"
#include "dbNet.hpp"

PROJECT_NAMESPACE_START

class CirDB {
  friend class Parser;
  friend class Ispd08Reader;
  friend class TechfileReader;
  friend class GdsReader;
 public:
  CirDB()
    : _name(""), _xl(0), _yl(0), _xh(0), _yh(0) {}
  ~CirDB() {}
  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  const String_t&   name()  const { return _name; }
  
  // Lef
  LefDB&            lef()         { return _lef; }
  const LefDB&      lef()   const { return _lef; }
 
  // Techfile
  TechfileDB&       tech()                                 { return _tech; }
  const TechfileDB& tech()                           const { return _tech; }
  UInt_t            layerIdx2MaskIdx(const UInt_t i) const;

  // Boundary
  Int_t             xl()     const { return _xl; }
  Int_t             yl()     const { return _yl; }
  Int_t             xh()     const { return _xh; }
  Int_t             yh()     const { return _yh; }
  Int_t             width()  const { return _xh - _xl; }
  Int_t             height() const { return _yh - _yl; }

  // Pin
  UInt_t            numPins()             const { return _vPins.size(); }
  Pin&              pin(const UInt_t i)         { return _vPins[i]; }
  const Pin&        pin(const UInt_t i)  const  { return _vPins[i]; }

  // Net
  UInt_t            numNets()                     const { return _vNets.size(); }
  UInt_t            str2NetIdx(const String_t& n) const { return _mStr2NetIdx.at(n); }
  Net&              net(const UInt_t i)                 { return _vNets[i]; }
  Net&              net(const String_t& n)              { return _vNets[str2NetIdx(n)]; }
  const Net&        net(const UInt_t i)           const { return _vNets[i]; }
  const Net&        net(const String_t& n)        const { return _vNets[str2NetIdx(n)]; }
  bool              hasNet(const String_t& n)     const { return _mStr2NetIdx.find(n) != _mStr2NetIdx.end(); }

  // Block
  UInt_t            numBlocks(const UInt_t i)               const { return _vvBlocks[i].size(); }
  Block&            block(const UInt_t i, const UInt_t j)         { return _vvBlocks[i][j]; }
  const Block&      block(const UInt_t i, const UInt_t j)   const { return _vvBlocks[i][j]; }

  // for debug
  void printInfo() const;
 private:
  String_t                       _name;
  LefDB                          _lef;
  TechfileDB                     _tech;

  Int_t                          _xl;
  Int_t                          _yl;
  Int_t                          _xh;
  Int_t                          _yh;
  
  Vector_t<Pin>                  _vPins;
  Vector_t<Net>                  _vNets;
  Vector_t<Vector_t<Block>>      _vvBlocks;

  UMap_t<String_t, UInt_t>      _mStr2NetIdx;
 
  //////////////////////////////////
  //  Private Setter              //
  //////////////////////////////////
  void setXL(const Int_t x);
  void setYL(const Int_t y);
  void setXH(const Int_t x);
  void setYH(const Int_t y);
  void addPin(const Pin& p);
  void addBlock(const UInt_t i, const Block& b);
  void addNet(const Net& n);
  void resizeVVBlocks(const UInt_t i);
};

////////////////////////////////////////
//   Iterators                        //
////////////////////////////////////////
// pins
#define Cir_ForEachPin(cir, pPin_, i) \
  for (i = 0; i < cir.numPins() and (pPin_ = &cir.pin(i)); ++i)

// nets
#define Cir_ForEachNet(cir, pNet_, i) \
  for (i = 0; i < cir.numNets() and (pNet_ = &cir.net(i)); ++i)

// blocks
#define Cir_ForEachBlock(cir, layerIdx, pBlock_, i) \
  for (i = 0; i < cir.numBlocks(layerIdx) and (pBlock = &cir.block(layerIdx, i)); ++i)

PROJECT_NAMESPACE_END

#endif /// _DB_CIRCUIT_HPP_
