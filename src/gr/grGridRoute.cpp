/**
 * @file   grGridRoute.cpp
 * @brief  Global Routing - Grid based Routing
 * @author Hao Chen
 * @date   10/07/2019
 *
 **/

#include "src/ds/pqueue.hpp"
#include "grGridRoute.hpp"
#include "grAstar.hpp"

PROJECT_NAMESPACE_START

void GrGridRoute::solve() {

  initGrids(_param.grid_x_scale, _param.grid_y_scale);
  markBlockedGrids();
  

  // inita nets in pqueue
  PairingHeap<Net*, Net_Cmp> pq;
  UInt_t i;
  Net* pNet;
  Cir_ForEachNet(_cir, pNet, i) {
    // ignore dangling net
    if (pNet->numPins() > 1) {
      //if (pNet->name() == "CLK")
      pq.push(pNet);
    }
  }
  // rip up and reroute
  Queue_t<Net*> qFrozenNet;
  while (!pq.empty() or !qFrozenNet.empty()) {
    while (qFrozenNet.size() > _rrParams.numFrozen) {
      pq.push(qFrozenNet.front());
      qFrozenNet.pop();
    }
    if (pq.empty()) {
      pq.push(qFrozenNet.front());
      qFrozenNet.pop();
    }
    pNet = pq.top();
    pq.pop();
    if (pNet->hasSymNet()) {
      if (pNet->symNetIdx() > pNet->idx()) {
        if (!routeSingleNet(*pNet, _rrParams.symNetWeight)) {
          pNet->addGrFail();
          qFrozenNet.push(pNet);
        }
      }
    }
    else if (pNet->bSelfSym()) {
      if (!routeSingleNet(*pNet, _rrParams.selfSymNetWeight)) {
        pNet->addGrFail();
        qFrozenNet.push(pNet);
      }
    }
    else {
      if (!routeSingleNet(*pNet, _rrParams.normalNetWeight)) {
        pNet->addGrFail();
        qFrozenNet.push(pNet);
      }
    }
  }


}

/////////////////////////////////////////
//    Private functions                //
/////////////////////////////////////////


/////////////////////////////////////////
//    Explicit Grid                    //
/////////////////////////////////////////
void GrGridRoute::initGrids(const Int_t scaleX, const Int_t scaleY) {
  // initialize grids
  // avoid using masterslice in GR
  const LefRoutingLayer& lefLayerM1 = _cir.lef().routingLayer(0);
  const Int_t minSpacing = lefLayerM1.spacingTable().table.size() ?
                           lefLayerM1.spacingTable().table[0].second[0] :
                           lefLayerM1.spacing(0);
  const Int_t stepX = minSpacing * scaleX;
  const Int_t stepY = minSpacing * scaleY;
  const UInt_t numZ = _cir.lef().numRoutingLayers();
  const UInt_t numX = _cir.width() / stepX;
  const UInt_t numY = _cir.height() / stepY;
  _gridMap.setNumZ(numZ);
  _gridMap.setNumX(numX);
  _gridMap.setNumY(numY);

  fprintf(stdout, "Global Routing Gridmap dim (%d %d %d)\n", numX, numY, numZ);
  
  UInt_t t, i, j, k;
  // set GridCell
  for (i = 0; i < _gridMap.numGridCellsZ(); ++i) {
    for (j = 0; j < _gridMap.numGridCellsX(); ++j) {
      for (k = 0; k < _gridMap.numGridCellsY(); ++k) {
        GrGridCell& gridCell = _gridMap.gridCell(i, j, k);
        gridCell.setWidth(minSpacing);
        gridCell.setHeight(minSpacing);
        gridCell.setZ(i);
        Point<Int_t> bl(_cir.xl() + j * stepX, _cir.yl() + k * stepY);
        Point<Int_t> tr(_cir.xl() + j * stepX + stepX, _cir.yl() + k * stepY + stepY);
        gridCell.setBox(bl, tr);
      }
    }
  }
  // set GridEdge
  for (t = 0; t < 3; ++t) {
    for (i = 0; i < _gridMap.numGridEdgesZ(t); ++i) {
      for (j = 0; j < _gridMap.numGridEdgesX(t); ++j) {
        for (k = 0; k < _gridMap.numGridEdgesY(t); ++k) {
          GrGridEdge& gridEdge = _gridMap.gridEdge(t, i, j, k);
          switch (t) {
            case 0: gridEdge.setMaxCap(scaleX); break;
            case 1: gridEdge.setMaxCap(scaleY); break;
            case 2: gridEdge.setMaxCap(MAX_UINT); break;
            default: assert(false);
          }
        }
      }
    }
  }
  // put pins into gridcells
  UInt_t layerIdx;
  Pin* pPin;
  const Box<Int_t>* cpBox;
  const Int_t offsetX = _cir.xl();
  const Int_t offsetY = _cir.yl();
  _vvNetPinLocs.resize(_cir.numNets());
  Cir_ForEachPin(_cir, pPin, i) {
    Pin_ForEachLayerIdx((*pPin), layerIdx) {
      const auto& layerPair = _cir.lef().layerPair(layerIdx);
      assert(layerPair.first == LefLayerType::MASTERSLICE or
             layerPair.first == LefLayerType::ROUTING);
      // avoid using masterslice in GR
      if (layerPair.first == LefLayerType::MASTERSLICE)
        continue;
      Pin_ForEachLayerBoxC((*pPin), layerIdx, cpBox, j) {
        Int_t xl = cpBox->xl() - offsetX;
        Int_t xh = cpBox->xh() - offsetX;
        Int_t yl = cpBox->yl() - offsetY;
        Int_t yh = cpBox->yh() - offsetY;
        UInt_t xlIdx = xl / stepX;
        UInt_t xhIdx = xh / stepX;
        UInt_t ylIdx = yl / stepY;
        UInt_t yhIdx = yh / stepY;
        if (xhIdx == _gridMap.numGridCellsX()) {
          --xhIdx;
        }
        if (yhIdx == _gridMap.numGridCellsY()) {
          --yhIdx;
        }
        assert(_cir.lef().bRoutingLayer(layerIdx));
        UInt_t routingLayerIdx = _cir.lef().layerPair(layerIdx).second;
        for (j = xlIdx; j <= xhIdx; ++j) {
          for (k = ylIdx; k <= yhIdx; ++k) {
            GrGridCell& targetCell = _gridMap.gridCell(routingLayerIdx, j, k);
            targetCell.addPinIdx(i);
            _vvNetPinLocs[pPin->netIdx()].emplace_back(j, k, routingLayerIdx);
          }
        }
      }
    }
  }
}

void GrGridRoute::markBlockedGrids() {
  UInt_t i, j, k, t;
  for (i = 0; i < _gridMap.numGridCellsZ(); ++i) {
    for (j = 0; j < _gridMap.numGridCellsX(); ++j) {
      for (k = 0; k < _gridMap.numGridCellsY(); ++k) {
        GrGridCell& gridCell = _gridMap.gridCell(i, j, k);
        Vector_t<UInt_t> vBlkIndices;
        Vector_t<UInt_t> vPinIndices;
        const Point<Int_t>& bl = gridCell.bl();
        const Point<Int_t>& tr = gridCell.tr();
        if (_cir.queryBlk(i, bl, tr, vBlkIndices)) {
          Int_t freeArea = gridCell.box().area();
          for (const UInt_t idx: vBlkIndices) {
            const Blk* cpBlk = &_cir.blk(idx);
            freeArea -= Box<Int_t>::overlapArea(gridCell.box(), cpBlk->box());
          }
          if (freeArea == 0) {
            gridCell.setInvalid();
          }
          UInt_t cap = std::round((Float_t)freeArea / gridCell.box().area());
          for (t = 0; t < 3; ++t) {
            if (i < _gridMap.numGridEdgesZ(t) and
                j < _gridMap.numGridEdgesX(t) and
                k < _gridMap.numGridEdgesY(t)) {
              GrGridEdge& gridEdge = _gridMap.gridEdge(t, i, j, k);
              gridEdge.setMaxCap(std::min(cap, gridEdge.maxCap()));
              if (i > 0) {
                GrGridEdge& prevEdge = _gridMap.gridEdge(2, i - 1, j, k);
                prevEdge.setMaxCap(std::min(cap, prevEdge.maxCap()));
              }
              if (j > 0) {
                GrGridEdge& prevEdge = _gridMap.gridEdge(0, i, j - 1, k);
                prevEdge.setMaxCap(std::min(cap, prevEdge.maxCap()));
              }
              if (k > 0) {
                GrGridEdge& prevEdge = _gridMap.gridEdge(1, i, j, k - 1);
                prevEdge.setMaxCap(std::min(cap, prevEdge.maxCap()));
              }
            }
          }
        }
      }
    }
  }
  //for (int j = _gridMap.numGridCellsY() - 1; j >= 0; --j) {
    //for (int k = 0; k < _gridMap.numGridCellsX(); ++k) {
      //if (j < _gridMap.numGridEdgesY(1)) {
        //GrGridEdge& gridEdge = _gridMap.gridEdge(1, 2, k, j);
        //printf("%d ", gridEdge.maxCap());
      //}
    //}
    //printf("\n");
    //for (int k = 0; k < _gridMap.numGridCellsX(); ++k) {
      //printf(".");
      //if (k < _gridMap.numGridEdgesX(0)) {
        //GrGridEdge& gridEdge = _gridMap.gridEdge(0, 2, k, j);
        //printf("%d", gridEdge.maxCap());
      //}
    //}
    //printf("\n");
  //}
}

/////////////////////////////////////////
//    Path Search Kernel               //
/////////////////////////////////////////
bool GrGridRoute::routeSingleNet(Net& n, const Int_t netWeight) {
  GrAstar astarKernel(_cir, n, netWeight, *this);
  return astarKernel.runKernel();
}

PROJECT_NAMESPACE_END

