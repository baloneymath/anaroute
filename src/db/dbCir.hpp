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
#include "dbBlock.hpp"
#include "dbPin.hpp"
#include "dbNets.hpp"

PROJECT_NAMESPACE_START

class CirDB {
  friend class Parser;
  friend class Ispd08Reader;
  friend class TechfileReader;
  friend class GdsReader;
public:
  CirDB()
    : _name("") {}
  ~CirDB() {}
  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  const String_t&   name()  const { return _name; }
  
  // Lef
  LefDB&            lef()         { return _lef; }
  const LefDB&      lef()   const { return _lef; }
  
  // Pin
  Index_t           numPins()             const { return _vPins.size(); }
  Pin&              pin(const Index_t i)        { return _vPins[i]; }
  const Pin&        pin(const Index_t i)  const { return _vPins[i]; }

  // for debug
  void printInfo() const;
private:
  String_t                       _name;
  LefDB                          _lef;
  Vector_t<Block>                _vBlocks;
  Vector_t<Pin>                  _vPins;
  Vector_t<DrNet>                _vDrNets;
 
  UMap_t<String_t, Index_t>      _mStr2LayerMaskIdx; // layer name -> layer idx specified in tsmc techfile
  //////////////////////////////////
  //  Private Setter              //
  //////////////////////////////////
  void addPin(const Pin& p);
  void addDrNet(const DrNet& n);
  void addStr2LayerMaxIdx(const String_t& n, const Index_t i);
};

PROJECT_NAMESPACE_END

#endif /// _DB_CIRCUIT_HPP_
