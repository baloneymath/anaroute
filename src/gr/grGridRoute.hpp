/**
 * @file   grGridRoute.hpp
 * @brief  Global Routing - Grid based Routing
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#ifndef _GR_GRID_ROUTE_HPP_
#define _GR_GRID_ROUTE_HPP_

#include "grMgr.hpp"
#include "grGridMap3d.hpp"

PROJECT_NAMESPACE_START

class GrGridRoute {
 public:
  GrGridRoute(GrMgr& mgr, CirDB& c)
    : _grMgr(mgr), _cir(c) {}
  ~GrGridRoute() {}
 
  void solve();

 private:
  GrMgr&      _grMgr;
  CirDB&      _cir;
  GrGridMap3d _gridMap;
  
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
  void initGrids(const Int_t scaleX, const Int_t scaleY); // dim = (scaleX * stepX), (scaleY * stepY)
  void markInvalidGrids();
};

PROJECT_NAMESPACE_END

#endif /// _GR_GRID_ROUTE_HPP_

