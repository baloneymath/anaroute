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
  Block(const Index_t i, const Box<Int_t>& b)
    : _layerIdx(i), _box(b) {}
  ~Block() {}
  
  ////////////////////////////////////////
  //   Getter                           //
  ////////////////////////////////////////
  Index_t                     layerIdx()                const { return _layerIdx; }
  const Box<Int_t>&           box()                     const { return _box; }
  

 private:
  Index_t               _layerIdx;
  Box<Int_t>            _box;
  
  ////////////////////////////////////////
  //   Setter                           //
  ////////////////////////////////////////
  void setLayerIdx(const Index_t i) { _layerIdx = i; }
  void setBox(const Box<Int_t>& b) { _box = b; }
};

PROJECT_NAMESPACE_END

#endif /// _DB_BLOCK_HPP_
