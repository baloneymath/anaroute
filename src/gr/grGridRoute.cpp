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

  initGrids(1, 1);

  markInvalidGrids();

  // inita nets in pqueue
  PairingHeap<Net*, Net_Cmp> pq;
  UInt_t i;
  Net* pNet;
  Cir_ForEachNet(_cir, pNet, i) {
    // ignore dangling net
    if (pNet->numPins() > 1) {
      pq.push(pNet);
    }
  }
  // rip up and reroute
  Queue_t<Net*> qFrozenNet;
  while (!pq.empty() or !qFrozenNet.empty()) {
    while (qFrozenNet.size() > _params.numFrozen) {
      pq.push(qFrozenNet.front());
      qFrozenNet.pop();
    }
    if (pq.empty()) {
      pq.push(qFrozenNet.front());
      qFrozenNet.pop();
    }
    pNet = pq.top();
    pq.pop();
  }


}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////
void GrGridRoute::initGrids(const Int_t scaleX, const Int_t scaleY) {
  UInt_t i, j, k;
  // initialize grids
  // avoid using masterslice in GR
  _gridMap.setNumZ(_cir.lef().numRoutingLayers());
  for (i = 0; i < _gridMap.numGridCellsZ(); ++i) {
    const LefRoutingLayer& lefLayer = _cir.lef().routingLayer(i);
    const Int_t minSpacing = lefLayer.spacingTable().table.size() ? 
                             lefLayer.spacingTable().table[0].second[0] :
                             lefLayer.spacing(0);
    const Int_t stepX = minSpacing * scaleX;
    const Int_t stepY = minSpacing * scaleY;
    _gridMap.setNumX(i, _cir.width() / stepX);
    _gridMap.setNumY(i, _cir.height() / stepY);
    for (j = 0; j < _gridMap.numGridCellsX(i); ++j) {
      for (k = 0; k < _gridMap.numGridCellsY(i); ++k) {
        GrGridCell& gridCell = _gridMap.gridCell(i, j, k);
        Int_t halfW = stepX / 2;
        Int_t halfH = stepY / 2;
        Int_t originX = _cir.xl() + halfW;
        Int_t originY = _cir.yl() + halfH;
        gridCell.setWidth(minSpacing);
        gridCell.setHeight(minSpacing);
        gridCell.setLoc(originX + j * stepX, originY + k * stepY, i);
      }
    }
  }
  // put pins into gridcells
  UInt_t layerIdx;
  Pin* pPin;
  const Box<Int_t>* cpBox;
  const Int_t offsetX = _cir.xl();
  const Int_t offsetY = _cir.yl();
  Cir_ForEachPin(_cir, pPin, i) {
    Pin_ForEachLayerIdx((*pPin), layerIdx) {
      const auto& layerPair = _cir.lef().layerPair(layerIdx);
      assert(layerPair.first == LefLayerType::MASTERSLICE or
             layerPair.first == LefLayerType::ROUTING);
      // avoid using masterslice in GR
      if (layerPair.first == LefLayerType::MASTERSLICE)
        continue;
      const LefRoutingLayer& lefLayer = _cir.lef().routingLayer(layerPair.second);
      const Int_t minSpacing = lefLayer.spacingTable().table.size() ? 
                               lefLayer.spacingTable().table[0].second[0] :
                               lefLayer.spacing(0);
      const Int_t stepX = minSpacing * scaleX;
      const Int_t stepY = minSpacing * scaleY;
      Pin_ForEachLayerBoxC((*pPin), layerIdx, cpBox, j) {
        Int_t xl = cpBox->xl() - offsetX;
        Int_t xh = cpBox->xh() - offsetX;
        Int_t yl = cpBox->yl() - offsetY;
        Int_t yh = cpBox->yh() - offsetY;
        UInt_t xlIdx = xl / stepX;
        UInt_t xhIdx = xh / stepX;
        UInt_t ylIdx = yl / stepY;
        UInt_t yhIdx = yh / stepY;
        if (xhIdx == _gridMap.numGridCellsX(layerIdx)) {
          --xhIdx;
        }
        if (yhIdx == _gridMap.numGridCellsY(layerIdx)) {
          --yhIdx;
        }
        for (j = xlIdx; j <= xhIdx; ++j) {
          for (k = ylIdx; k <= yhIdx; ++k) {
            GrGridCell& targetCell = _gridMap.gridCell(layerIdx, j, k);
            targetCell.addPinIdx(i);
          }
        }
      }
    }
  }
  // for debug
  //printf("GRIDMAP\n");
  //for (i = 0; i < _gridMap.numGridCellsZ(); ++i) {
    //printf("(%d %d %d)\n", _gridMap.numGridCellsX(i), _gridMap.numGridCellsY(i), i);
    //for (j = 0; j < _gridMap.numGridCellsX(i); ++j) {
      //for (k = 0; k < _gridMap.numGridCellsY(i); ++k) {
        //GrGridCell& gridCell = _gridMap.gridCell(i, j, k);
        //for (const UInt_t idx : gridCell.vPinIndices()) {
          //printf("GRIDCELL (%d %d %d) (%d %d %d)", j, k, i, gridCell.x(), gridCell.y(), gridCell.z());
          //printf(" PIN %d", idx);
          //printf("\n");
        //}
      //}
    //}
  //}

}

void GrGridRoute::markInvalidGrids() {
  UInt_t i, j, k, layerIdx;
  const Blk* cpBlk;
  for (i = 0; i < _gridMap.numGridCellsZ(); ++i) {
    for (j = 0; j < _gridMap.numGridCellsX(i); ++j) {
      for (k = 0; k < _gridMap.numGridCellsY(i); ++k) {
        GrGridCell& gridCell = _gridMap.gridCell(i, j, k);
        Vector_t<UInt_t> vBlkIndices;
        Vector_t<UInt_t> vPinIndices;
        Point<Int_t> loc(gridCell.x(), gridCell.y());
        if (_cir.queryBlk(i, loc, loc, vBlkIndices)) {
          gridCell.setInvalid();
        }
      }
    }
  }
  //for (j = _gridMap.numGridCellsY(2) - 1; j >= 0; --j) {
    //for (k = 0; k < _gridMap.numGridCellsX(2); ++k) {
      //GrGridCell& gridCell = _gridMap.gridCell(2, k, j);
      //if (gridCell.bValid())
        //printf(".");
      //else
        //printf("X");
    //}
    //printf("\n");
  //}
}

void GrGridRoute::routeSingleNet() {

}

void GrGridRoute::routeSingleSymNet() {

}

void GrGridRoute::routeSingleSelfSymNet() {

}

PROJECT_NAMESPACE_END

