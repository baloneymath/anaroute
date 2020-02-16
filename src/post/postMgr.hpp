/**
 * @file   postMgr.hpp
 * @brief  Post DR processing - Manager
 * @author Hao Chen
 * @date   02/15/2020
 *
 **/

#ifndef _POST_MGR_HPP_
#define _POST_MGR_HPP_

#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class PostMgr {
 public:
  PostMgr(CirDB& c)
    : _cir(c) {}
  ~PostMgr() {}
 
 private:
  CirDB& _cir;
};

PROJECT_NAMESPACE_END

#endif /// _POST_MGR_HPP_
