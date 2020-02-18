/**
 * @file   postMgr.cpp
 * @brief  Post DR processing - Manager
 * @author Hao Chen
 * @date   02/15/2020
 *
 **/

#include "postMgr.hpp"
#include "src/geo/segment.hpp"

PROJECT_NAMESPACE_START

void PostMgr::solve() {
  patchJogs();
}

void PostMgr::patchJogs() {
  // separate shapes of each layer
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes;
  Vector_t<Vector_t<Polygon<Int_t>>> vvPolygons;
  vvBoxes.resize(_cir.lef().numLayers());
  vvPolygons.resize(_cir.lef().numLayers());
  // add boxes (pin shapes and routed wires)
  UInt_t i, j;
  const Net* pNet;
  const Blk* pBlk;
  const Pair_t<Box<Int_t>, Int_t>* pWire;

  Cir_ForEachBlk(_cir, pBlk, i) {
    vvBoxes[pBlk->layerIdx()].emplace_back(pBlk->box());
  }
  Cir_ForEachNet(_cir, pNet, i) {
    Net_ForEachRoutedWire((*pNet), pWire, j) {
      const auto& box = pWire->first;
      const Int_t layerIdx = pWire->second;
      // don't need to add cut layer shapes
      if (_cir.lef().bRoutingLayer(layerIdx)) {
        vvBoxes[layerIdx].emplace_back(box);
      }
    }
  }
  // transform boxes to polygons for each layer
  Int_t k = 0;
  for (i = 0; i < vvBoxes.size(); ++i) {
    const auto& vBoxes = vvBoxes.at(i);
    if (vBoxes.empty())
      continue;
    assert(_cir.lef().bRoutingLayer(i));
    const auto& layerPair = _cir.lef().layerPair(i);
    const auto& layer = _cir.lef().routingLayer(layerPair.second);
    // if no constraint
    if (layer.numMinSteps() == 0)
      continue;

    // FIXME: only handle our PDK condition currently
    assert(layer.numMinSteps() == 1);

    auto& vPolygons = vvPolygons[i];
    geo::box2Polygon<Int_t>(vBoxes, vPolygons);
    for (const auto& polygon : vPolygons) {
      const auto& ring = polygon.outer();
      for (j = 1; j < ring.size() - 1; ++j) {
        const auto& pt0 = ring[j - 1];
        const auto& pt1 = ring[j];
        const auto& pt2 = ring[j + 1];
        Segment<Int_t> edge1(pt0, pt1);
        Segment<Int_t> edge2(pt1, pt2);
        assert((edge1.bHorizontal() and edge2.bVertical())
               or (edge1.bVertical() and edge2.bHorizontal()));
        if (edge1.length() < layer.minStep(0)
            and edge2.length() < layer.minStep(0)) {
          // TODO
          fprintf(stderr, "!!!! %d\n", ++k);
        }
      }
    }
  }
}

PROJECT_NAMESPACE_END
