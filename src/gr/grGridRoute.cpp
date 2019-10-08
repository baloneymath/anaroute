/**
 * @file   drGridRoute.cpp
 * @brief  Detailed Routing - Grid based Routing
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#include "grGridRoute.hpp"
#include "src/ds/pqueue.hpp"

PROJECT_NAMESPACE_START

void GrGridRoute::solve() {
  UInt_t i;

  initGrids();

  PairingHeap<Net*, Net_Cmp> pqueue;
  
  Net* pNet;
  Cir_ForEachNet(_cir, pNet, i) {
    // ignore dangling net
    if (pNet->numPins() > 1) {
      pqueue.push(pNet);
    }
  }


}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////
void GrGridRoute::initGrids() {
  UInt_t i, j, k;
  // initialize grids
  _gridMap.setNumZ(_cir.lef().numRoutingLayers());
  for (i = 0; i < _gridMap.numGridCellsZ(); ++i) {
    const LefRoutingLayer& lefLayer = _cir.lef().routingLayer(i);
    const Int_t minSpacing = lefLayer.spacingTable().table.size() ? 
                             lefLayer.spacingTable().table[0].second[0] :
                             lefLayer.spacing(0);
    _gridMap.setNumX(i, _cir.width() / minSpacing);
    _gridMap.setNumY(i, _cir.height() / minSpacing);
    for (j = 0; j < _gridMap.numGridCellsX(i); ++j) {
      for (k = 0; k < _gridMap.numGridCellsY(i); ++k) {
        GrGridCell& gridCell = _gridMap.gridCell(i, j, k);
        Int_t halfW = minSpacing / 2;
        Int_t halfH = minSpacing / 2;
        gridCell.setWidth(minSpacing);
        gridCell.setHeight(minSpacing);
        gridCell.setLoc(halfW + j * minSpacing, halfH + k * minSpacing, i);
      }
    }
  }
  // put pins
  Pin* pPin;
  Cir_ForEachPin(_cir, pPin, i) {
    
  }
}

PROJECT_NAMESPACE_END

