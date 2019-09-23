/**
 * @file   ispd08reader.hpp
 * @brief  ISPD08-like netlist format parser
 * @author Hao Chen
 * @date   09/22/2019
 *
 **/

#ifndef _ISPD08_READER_HPP_
#define _ISPD08_READER_HPP_

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class Ispd08Reader {
  public:
   Ispd08Reader(CirDB& c)
    : _cir(c) {}
   ~Ispd08Reader() {}
    
   void parse(const String_t& filename);
  private:
   CirDB& _cir;
};

PROJECT_NAMESPACE_END

#endif /// _ISPD08_READER_HPP_
