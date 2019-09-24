/**
 * @file   parser.hpp
 * @brief  Parser manager for database
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"
#include "lefreader.hpp"
#include "ispd08reader.hpp"
#include "gdsReader.hpp"

PROJECT_NAMESPACE_START

class Parser {
public:
  Parser(CirDB& c)
    : _cir(c), _lefr(c.lef()), _ispd08r(c), _gdsr(c) {}
  ~Parser() {}
  
  void parseLef(const String_t& filename) { _lefr.parse(filename); }
  void parseIspd08(const String_t& filename) { _ispd08r.parse(filename); }

private:
  CirDB&           _cir;
  LefReader        _lefr;
  Ispd08Reader     _ispd08r;
  GdsReader        _gdsr;
};

PROJECT_NAMESPACE_END

#endif /// _PARSER_HPP_
