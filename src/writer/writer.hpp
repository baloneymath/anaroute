/**
 * @file   writer.hpp
 * @brief  Output file writer
 * @author Hao Chen
 * @date   10/18/2019
 *
 **/

#ifndef _WRITER_HPP_
#define _WRITER_HPP_

#include "wrGrGuide.hpp"
#include "wrGds.hpp"

PROJECT_NAMESPACE_START

class Writer {
 public:
  Writer(const CirDB& c)
    : _cir(c), _grGuidew(c) {}
  ~Writer() {}

  // guide
  void writeGrGuide(const String_t& filename) const { _grGuidew.write(filename); }
  void writeGrGuideGds(const String_t& netName, const String_t& filename) const { _grGuidew.writeGds(netName, filename); }
    
 private:
  const CirDB&  _cir;
  GrGuideWriter _grGuidew;
};

PROJECT_NAMESPACE_END

#endif /// _WRITER_HPP_
