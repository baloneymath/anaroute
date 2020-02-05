/**
 * @file   drGridRoute.hpp
 * @brief  Detailed Routing - Grid Based Routing
 * @author Hao Chen
 * @date   02/01/2020
 *
 **/

#ifndef _DR_GRID_ROUTE_HPP_
#define _DR_GRID_ROUTE_HPP_

#include "drMgr.hpp"

PROJECT_NAMESPACE_START

class DrGridRoute {
  friend class DrGridAstar;
 public:
  DrGridRoute(CirDB& c, DrMgr& dr, DrcMgr& drc)
    : _cir(c), _drMgr(dr), _drcMgr(drc) {}
  ~DrGridRoute() {}

  void solve();

 private:
  CirDB&    _cir;
  DrMgr&    _drMgr;
  DrcMgr&   _drcMgr;
  
  /////////////////////////////////////////
  //    Private structs                  //
  /////////////////////////////////////////
  struct Net_Cmp {
    bool operator() (const Net* pn1, const Net* pn2) {
      Int_t c1 = w_pin_cnt  * pn1->numPins() +
                 w_sym      * pn1->hasSymNet() +
                 w_selfSym  * pn1->bSelfSym() +
                 w_fail_cnt * pn1->drFailCnt();
      Int_t c2 = w_pin_cnt  * pn2->numPins() +
                 w_sym      * pn2->hasSymNet() +
                 w_selfSym  * pn2->bSelfSym() +
                 w_fail_cnt * pn2->drFailCnt();
      return c1 < c2;
    }
    Int_t w_pin_cnt = 1;
    Int_t w_sym = 100;
    Int_t w_selfSym = 200;
    Int_t w_fail_cnt = 100;
  };
  
  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
  bool routeSingleNet(Net& n);
  void ripupSingleNet(Net& n);
};

PROJECT_NAMESPACE_END

#endif /// _DR_GRID_ROUTE_HPP_
