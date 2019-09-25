/**
 * @file   techfileReader.hpp
 * @brief  Parser - Tsmc techfile (techlayer only)
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#ifndef _TECHFILE_READER_HPP_
#define _TECHFILE_READER_HPP_

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class TechfileReader {
 public:
  TechfileReader(CirDB& c)
    : _cir(c) {}
  ~TechfileReader() {}

  void parse(const String_t& filename);

 private:
  CirDB& _cir;
  
  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
  void readTechLayers(FILE* fin, char* buf, const Index_t bufSize);
};

PROJECT_NAMESPACE_END

#endif /// _TECHFILE_READER_HPP_
