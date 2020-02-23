/**
 * @file   drRoutable.hpp
 * @brief  Net routable construction
 * @author Hao Chen
 * @date   02/21/2020
 *
 **/

#ifndef _DR_ROUTABLE_HPP_
#define _DR_ROUTABLE_HPP_

#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class DrRoutable {
 public:
  DrRoutable(CirDB& c)
    : _cir(c) {}
  ~DrRoutable() {}

  void constructRoutables();

  void constructNetRoutables(Net& net);
  void constructNormalNetRoutables(Net& net);
  void constructSelfSymNetRoutables(Net& net);
  void constructSymNetRoutables(Net& net);

 private:
  CirDB& _cir;
};

PROJECT_NAMESPACE_END

#endif /// _DR_ROUTABLE_HPP_
