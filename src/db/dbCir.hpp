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
public:
  CirDB() {}
  ~CirDB() {}
  //////////////////////////////////
  //  Getter                      //
  //////////////////////////////////
  LefDB&       lef()       { return _lef; }
  const LefDB& lef() const { return _lef; }
private:
  LefDB _lef;
};

PROJECT_NAMESPACE_END

#endif /// _DB_CIRCUIT_HPP_
