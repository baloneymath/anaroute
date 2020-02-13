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
#include "src/geo/spatial.hpp"

PROJECT_NAMESPACE_START

class DrGridRoute {
  friend class DrGridAstar;
 public:
  DrGridRoute(CirDB& c, DrMgr& dr, DrcMgr& drc)
    : _cir(c), _drMgr(dr), _drc(drc),
      _vSpatialHistoryMaps(c.lef().numLayers()) {}
  ~DrGridRoute() {}

  void solve();

 private:
  CirDB&    _cir;
  DrMgr&    _drMgr;
  DrcMgr&   _drc;
  
  Vector_t<SpatialMap<Int_t, Int_t>> _vSpatialHistoryMaps;

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

  struct Param {
    Int_t maxSymTry = 3;
    Int_t maxSelfSymTry = 3;
  } _param;
  
  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
  void checkSymSelfSym(const Net& net, bool& bSym, bool& bSelfSym);
  
  bool routeSingleNet(Net& n, const bool bSym, const bool bSelfSym, const bool bStrictDRC);

  bool checkDRC();
  bool checkSingleNetDRC(const Net& net);
  
  void ripupSingleNet(Net& n);
  
  void addWireHistoryCost(const Int_t cost, const Int_t layerIdx, const Box<Int_t>& wire);
  void addViaHistoryCost(const Int_t cost, const Int_t x, const Int_t y, const LefVia& via);

  bool bSatisfySymCondition(const Net& net);
  bool bSatisfySelfSymCondition(const Net& net);

};

PROJECT_NAMESPACE_END

#endif /// _DR_GRID_ROUTE_HPP_
