/**
 * @file   dbBlock.hpp
 * @brief  Circuit Element - Blockage
 * @author Hao Chen
 * @date   09/20/2019
 *
 **/

#ifndef _DB_BLOCK_HPP_
#define _DB_BLOCK_HPP_

#include "src/global/global.hpp"
#include "src/geo/box.hpp"

PROJECT_NAMESPACE_START

class Block {
  friend class Parser;
 public:
  Block()
    : _layerIdx(MAX_INDEX) {}
  ~Block() {}
  
  ////////////////////////////////////////
  //   Getter                           //
  ////////////////////////////////////////
  Index_t                     layerIdx()                const { return _layerIdx; }
  Index_t                     numBoxes(const Index_t i) const { return _vBoxes.size(); }
  const Box<Int_t>&           box(const Index_t i)      const { return _vBoxes[i]; }
  const Vector_t<Box<Int_t>>& vBoxes()                  const { return _vBoxes; }
  

 private:
  Index_t               _layerIdx;
  Vector_t<Box<Int_t>>  _vBoxes;
  
  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setLayerIdx(const Index_t i) { _layerIdx = i; }
  void addBox(const Box<Int_t>& b) { _vBoxes.emplace_back(b); }
};

PROJECT_NAMESPACE_END

#endif /// _DB_BLOCK_HPP_
