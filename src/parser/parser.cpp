/**
 * @file   parser.cpp
 * @brief  Parser manager for database
 * @author Hao Chen
 * @date   10/27/2019
 *
 **/

#include "parser.hpp"

PROJECT_NAMESPACE_START

// patch for placement bugs .... orz
void Parser::correctPinNBlkLoc() {
  UInt_t i, j, layerIdx;
  // pins
  Pin* pPin;
  Cir_ForEachPin(_cir, pPin, i) {
    Pin_ForEachLayerIdx((*pPin), layerIdx) {
      //Pin_ForEachLayerBox((*pPin), layerIdx, pBox, j) {
      for (j = 0; j < pPin->numBoxes(layerIdx); ++j) {
        Box<Int_t>* pBox = &pPin->_vvBoxes[layerIdx][j];
        assert(pBox->xl() % 10 == 0 or pBox->xl() % 10 == 8);
        assert(pBox->yl() % 10 == 0 or pBox->yl() % 10 == 8);
        assert(pBox->xh() % 10 == 0 or pBox->xh() % 10 == 8);
        assert(pBox->yh() % 10 == 0 or pBox->yh() % 10 == 8);
        if (pBox->xl() % 10 == 8) {
          pBox->setXL(pBox->xl() + 2);
        }
        if (pBox->yl() % 10 == 8) {
          pBox->setYL(pBox->yl() + 2);
        }
        if (pBox->xh() % 10 == 8) {
          pBox->setXH(pBox->xh() + 2);
        }
        if (pBox->yh() % 10 == 8) {
          pBox->setYH(pBox->yh() + 2);
        }
      }
    }
  }

  // blks
  Blk* pBlk;
  Cir_ForEachBlk(_cir, pBlk, i) {
    Box<Int_t>* pBox = &pBlk->_box;
    assert(pBox->xl() % 10 == 0 or pBox->xl() % 10 == 8);
    assert(pBox->yl() % 10 == 0 or pBox->yl() % 10 == 8);
    assert(pBox->xh() % 10 == 0 or pBox->xh() % 10 == 8);
    assert(pBox->yh() % 10 == 0 or pBox->yh() % 10 == 8);
    if (pBox->xl() % 10 == 8) {
      pBox->setXL(pBox->xl() + 2);
    }
    if (pBox->yl() % 10 == 8) {
      pBox->setYL(pBox->yl() + 2);
    }
    if (pBox->xh() % 10 == 8) {
      pBox->setXH(pBox->xh() + 2);
    }
    if (pBox->yh() % 10 == 8) {
      pBox->setYH(pBox->yh() + 2);
    }
  }
}

PROJECT_NAMESPACE_END
