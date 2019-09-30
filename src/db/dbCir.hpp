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
  TechfileDB&       tech()                                  { return _tech; }
  const TechfileDB& tech()                            const { return _tech; }
  Index_t           layerIdx2MaskIdx(const Index_t i) const;

  // Boundary
  Int_t             xl()    const { return _xl; }
  Int_t             yl()    const { return _yl; }
  Int_t             xh()    const { return _xh; }
  Int_t             yh()    const { return _yh; }

  // Pin
  Index_t           numPins()             const { return _vPins.size(); }
  Pin&              pin(const Index_t i)        { return _vPins[i]; }
  const Pin&        pin(const Index_t i)  const { return _vPins[i]; }

  // Net
  Index_t           numNets()                     const { return _vNets.size(); }
  Index_t           str2NetIdx(const String_t& n) const { return _mStr2NetIdx.at(n); }
  Net&              net(const Index_t i)                { return _vNets[i]; }
  Net&              net(const String_t& n)              { return _vNets[str2NetIdx(n)]; }
  const Net&        net(const Index_t i)          const { return _vNets[i]; }
  const Net&        net(const String_t& n)        const { return _vNets[str2NetIdx(n)]; }
  bool              hasNet(const String_t& n)     const { return _mStr2NetIdx.find(n) != _mStr2NetIdx.end(); }

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

  UMap_t<String_t, Index_t>      _mStr2NetIdx;
 
  //////////////////////////////////
  //  Private Setter              //
  //////////////////////////////////
  void setXL(const Int_t x);
  void setYL(const Int_t y);
  void setXH(const Int_t x);
  void setYH(const Int_t y);
  void addPin(const Pin& p);
  void addBlock(const Index_t i, const Block& b);
  void addNet(const Net& n);
  void resizeVVBlocks(const Index_t i);
};

PROJECT_NAMESPACE_END

#endif /// _DB_CIRCUIT_HPP_
