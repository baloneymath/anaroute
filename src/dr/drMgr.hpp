/**
 * @file   drMgr.hpp
 * @brief  Detailed Routing - Manager
 * @author Hao Chen
 * @date   09/30/2019
 *
 **/

#ifndef _DR_MGR_HPP_
#define _DR_MGR_HPP_

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class DrMgr {
 public:
  DrMgr(CirDB& c)
    : _cir(c) {}
  ~DrMgr() {}

  void solve();

 private:
  CirDB& _cir;
  /////////////////////////////////////////
  //    Private structs                  //
  /////////////////////////////////////////
  // for net comparison in pqueue
  struct Net_Cmp {
    bool operator() (const Net* pn1, const Net* pn2) {
      Int_t c1 = w_pin_cnt  * pn1->numPins() +
                 w_sym      * pn1->hasSymNet() +
                 w_selfSym  * pn1->bSelfSym() +
                 w_fail_cnt * pn1->failCnt();
      Int_t c2 = w_pin_cnt  * pn2->numPins() +
                 w_sym      * pn2->hasSymNet() +
                 w_selfSym  * pn2->bSelfSym() +
                 w_fail_cnt * pn2->failCnt();
      return c1 < c2;
    }
    Int_t w_pin_cnt = 1;
    Int_t w_sym = 1;
    Int_t w_selfSym = 1;
    Int_t w_fail_cnt = 1;
  };
  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
};


PROJECT_NAMESPACE_END

#endif /// _DR_MGR_HPP_
