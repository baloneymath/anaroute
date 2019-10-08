/**
 * @file   drGridlessRoute.hpp
 * @brief  Detailed Routing - Gridless Routing
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#ifndef _DR_GRIDLESS_ROUTE_HPP_
#define _DR_GRIDLESS_ROUTE_HPP_

#include "drMgr.hpp"

PROJECT_NAMESPACE_START

class DrGridlessRoute {
 public:
  DrGridlessRoute(DrMgr& mgr, CirDB& c)
    : _drMgr(mgr), _cir(c) {}
  ~DrGridlessRoute() {}

  void solve();

 private:
  DrMgr& _drMgr;
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

#endif /// _DR_GRIDLESS_ROUTE_HPP_

