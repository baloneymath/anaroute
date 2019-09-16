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

PROJECT_NAMESPACE_START

class Parser {
public:
  Parser(CirDB& c)
    : _cir(c), _lefr(c.lef()){}
  ~Parser() {}
  
  void parseLef(const String_t& lefFilename) {
    _lefr.parse(lefFilename);
  }

private:
  CirDB&           _cir;
  LefReader        _lefr;
};

PROJECT_NAMESPACE_END

#endif /// _PARSER_HPP_
