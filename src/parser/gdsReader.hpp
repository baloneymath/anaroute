/**
 * @file   GdsReader.hpp
 * @brief  Parser - Gds format parser
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#ifndef _GDS_READER_HPP_
#define _GDS_READER_HPP_

#include <limbo/parsers/gdsii/gdsdb/GdsIO.h>
#include <limbo/parsers/gdsii/gdsdb/GdsObjectHelpers.h>

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class GdsReader {
 public:
  GdsReader(CirDB& c)
    : _cir(c) {}
  ~GdsReader() {}

 private:
  CirDB& _cir; 
};

PROJECT_NAMESPACE_END

#endif /// _GDS_READER_HPP_
