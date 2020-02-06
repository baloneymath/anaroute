/**
 * @file   drGridAstarNode.hpp
 * @brief  Detailed Routing - Grid-Based A* Node
 * @author Hao Chen
 * @date   02/05/2020
 *
 **/

#ifndef _DR_GRID_ASTAR_NODE_HPP_
#define _DR_GRID_ASTAR_NODE_HPP_

#include "src/geo/point3d.hpp"

PROJECT_NAMESPACE_START

class DrGridAstarNode {
 public:
  DrGridAstarNode() {}
  ~DrGridAstarNode() {}

 private:
  Point3d<Int_t>              _coord;
  Int_t                       _costG;
  Int_t                       _costF;
  Int_t                       _bendCnt;
  
  bool                        _bExplored;
  DrGridAstarNode*            _pParent;
  Int_t                       _dist2Tar;
  Vector_t<DrGridAstarNode*>  _vpNeighbors;
};

PROJECT_NAMESPACE_END

#endif /// _DR_GRID_ASTAR_NODE_HPP_
