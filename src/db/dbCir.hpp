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
 
  // Techfile
  TechfileDB&       tech()                                  { return _tech; }
  const TechfileDB& tech()                            const { return _tech; }
  Index_t           layerIdx2MaskIdx(const Index_t i) const;

  // Pin
  Index_t           numPins()             const { return _vPins.size(); }
  Pin&              pin(const Index_t i)        { return _vPins[i]; }
  const Pin&        pin(const Index_t i)  const { return _vPins[i]; }

  // for debug
  void printInfo() const;
private:
  String_t                       _name;
  LefDB                          _lef;
  TechfileDB                     _tech;
  Vector_t<Pin>                  _vPins;
  Vector_t<DrNet>                _vDrNets;
  Vector_t<Vector_t<Block>>      _vvBlocks;
 
  //////////////////////////////////
  //  Private Setter              //
  //////////////////////////////////
  void addPin(const Pin& p);
  void addBlock(const Index_t i, const Block& b);
  void addDrNet(const DrNet& n);
  void resizeVVBlocks(const Index_t i);
};

PROJECT_NAMESPACE_END

#endif /// _DB_CIRCUIT_HPP_
