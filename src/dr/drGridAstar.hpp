/**
 * @file   drGridAstar.hpp
 * @brief  Detailed Routing - Grid-Based A* search kernel
 * @author Hao Chen
 * @date   02/05/2020
 *
 **/

#ifndef _DR_GRID_ASTAR_HPP_
#define _DR_GRID_ASTAR_HPP_

#include "drGridRoute.hpp"
#include "drGridAstarNode.hpp"
#include "src/ds/disjointSet.hpp"
#include "src/ds/hash.hpp"
#include "src/geo/point3d.hpp"
#include "src/geo/spatial.hpp"

PROJECT_NAMESPACE_START

class DrGridAstar {
 public:
  DrGridAstar(CirDB& c, Net& n, DrcMgr& d, DrGridRoute& dr,
              const bool bSym, const bool bSelfSym, const bool bStrictDRC)
    : _cir(c), _net(n), _drc(d), _dr(dr),
      _bSym(bSym), _bSelfSym(bSelfSym), _bStrictDRC(bStrictDRC) {
    _vAllNodesMap.resize(_cir.lef().numLayers());
    for (auto& m : _vAllNodesMap) {
      m.set_empty_key(Point<Int_t>(-1, -1));
    }
    _param.viaCost = _cir.gridStep() * 3;
  }
  
  ~DrGridAstar() {
    for (auto& v : _vAllNodesMap) {
      for (auto& m : v) {
        delete m.second;
      }
    }
  }

  bool run();

 private:
  CirDB&        _cir;
  Net&          _net;
  DrcMgr&       _drc;
  DrGridRoute&  _dr;
  const bool    _bSym;
  const bool    _bSelfSym;
  const bool    _bStrictDRC;

  // components
  DisjointSet                                                     _compDS;
  Vector_t<Vector_t<Pair_t<Box<Int_t>, Int_t>>>                   _vCompBoxes;
  Vector_t<DenseHashSet<Point3d<Int_t>, Point3d<Int_t>::hasher>>  _vCompAcsPts;
  Vector_t<UMap_t<Int_t, Spatial<Int_t>>>                         _vCompSpatialBoxes;
  Vector_t<Pair_t<UInt_t, UInt_t>>                                _vSubNets;

  // astar
  // mem add in routeSubNet(), free in destructor
  Vector_t<DenseHashMap<Point<Int_t>, DrGridAstarNode*, Point<Int_t>::hasher>> _vAllNodesMap;

  // result
  Vector_t<Vector_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>>  _vvRoutePaths;
  Vector_t<Vector_t<Pair_t<Box<Int_t>, Int_t>>>               _vvRoutedWires;

  struct Param {
    Int_t horCost = 1;
    Int_t verCost = 1;
    Int_t viaCost; // dynamic determined by gridStep
    Int_t factorG = 1;
    Int_t factorH = 1;
    Int_t guideCost = -5000;
    Int_t drcCost = 100000;
    Int_t historyCost = 500; // the cost added to the history map
    Int_t maxExplore = 200000;
  } _param;
  
  enum class PathDir : Byte_t {
    LEFT      = 0,
    RIGHT     = 1,
    UP        = 2,
    DOWN      = 3,
    VIA_UP    = 4,
    VIA_DOWN  = 5
  };

  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
  void  init();
  void  splitSubNetMST();
  bool  route(const bool bSym, const bool bSelfSym);
  bool  routeSubNet(Int_t srcIdx, Int_t tarIdx, const bool bSym, const bool bSelfSym);
  bool  pathSearch(const Int_t srcIdx, const Int_t tarIdx, const bool bSym, const bool bSelfSym);
  Int_t mergeComp(const Int_t srcIdx, const Int_t tarIdx);
  void  backTrack(const DrGridAstarNode* pU, const Int_t bigCompIdx, const Int_t srcIdx, const Int_t tarIdx, const bool bSym);
  void  add2Path(const DrGridAstarNode* pU, List_t<Point3d<Int_t>>& lPathPts);
  void  mergePath(const List_t<Point3d<Int_t>>& lPathPts, List_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>& lPathVec);
  bool  bNeedMergePath(const Point3d<Int_t>& u1, const Point3d<Int_t>& v1, const Point3d<Int_t>& u2, const Point3d<Int_t>& v2);
  void  addAcsPts(const Int_t idx, const Int_t z, const Box<Int_t>& box);
  void  findNeighbors(DrGridAstarNode* pU);
  bool  bViolateDRC(const DrGridAstarNode* pU, const DrGridAstarNode* pV, const bool bSym, const bool bSelfSym);
  bool  checkMinArea(const DrGridAstarNode* pU, const DrGridAstarNode* pV);
  bool  bNeedUpdate(const DrGridAstarNode* pV, const Int_t costG, const Int_t bendCnt);
  void  resetAllNodes();
  void  savePath(const List_t<Pair_t<Point3d<Int_t>, Point3d<Int_t>>>& lPathVec, const bool bSym);
  void  via2LayerBoxes(const Int_t x, const Int_t y, const LefVia& via, Vector_t<Pair_t<Box<Int_t>, Int_t>>& vLayerBoxes);
  void  saveResult2Net(const bool bSym);
  void  ripup();

  Int_t   scaledMDist(const Point3d<Int_t>& u, const Point3d<Int_t>& v);
  Int_t   scaledMDist(const Box<Int_t>& u, const Box<Int_t>& v);
  Int_t   scaledMDist(const Pair_t<Box<Int_t>, Int_t>& u, const Pair_t<Box<Int_t>, Int_t>& v);
  Int_t   scaledMDist(const Point3d<Int_t>& u, const Pair_t<Box<Int_t>, Int_t>& box);

  bool    hasBend(const DrGridAstarNode* pU, const DrGridAstarNode* pV);
  PathDir findDir(const Point3d<Int_t>& u, const Point3d<Int_t>& v);
  bool    bInsideGuide(const DrGridAstarNode* pU);
  Int_t   history(const DrGridAstarNode* pU);
  void    toWire(const Point3d<Int_t>& u, const Point3d<Int_t>& v, const Int_t width, const Int_t extension, Box<Int_t>& wire);
};


PROJECT_NAMESPACE_END

#endif /// _DR_GRID_ASTAR_HPP_
