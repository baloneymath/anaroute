/**
 * @file   parser.hpp
 * @brief  Parser manager for database
 * @author Hao Chen
 * @date   09/09/2019
 *
 **/

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "include/cmdline.h"
#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"
#include "lefreader.hpp"

PROJECT_NAMESPACE_START

class Parser {
public:
  Parser(cmdline::parser& p, CirDB& c)
    : _args(p), _cir(c), _lefr(c.lef()){}
  ~Parser() {}
  
  void parse() {
    _lefr.parse(_args.get<String_t>("tech_lef"));
  }
private:
  cmdline::parser& _args;
  CirDB&           _cir;
  LefReader        _lefr;
};

PROJECT_NAMESPACE_END

#endif /// _PARSER_HPP_
