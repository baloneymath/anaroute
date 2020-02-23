/**
 * @file   drRoutable.cpp
 * @brief  Net routable construction
 * @author Hao Chen
 * @date   02/21/2020
 *
 **/

#include "drRoutable.hpp"

using namespace std;

PROJECT_NAMESPACE_START

void DrRoutable::constructRoutables() {
  for (Int_t i = 0; i < (Int_t)_cir.numNets(); ++i) {
    Net& net = _cir.net(i);
    constructNetRoutables(net);
    //cerr << "Net: " << net.name() << endl;
    //cerr << "Pins: ";
    //for (auto pinIdx : net.vPinIndices())
      //cerr << pinIdx << " ";
    //cerr << endl;
    //for (Int_t j = 0; j < net.numRoutables(); ++j) {
      //cerr << "  Routable: " << j << endl;
      //cerr << "    Pins: ";
      //for (Int_t k = 0; k < net.routable(j).numPins(); ++k)
        //cerr << net.routable(j).pinIdx(k) << " ";
      //cerr << endl;
      //cerr << "    Routables: ";
      //for (Int_t k = 0; k < net.routable(j).numRoutables(); ++k)
        //cerr << net.routable(j).routableIdx(k) << " ";
      //cerr << endl;
    //}
    //cerr << endl;
  }
}

void DrRoutable::constructNetRoutables(Net& net) {
  if (!net.bSelfSym() and !net.hasSymNet()) {
    constructNormalNetRoutables(net);
  }
  else if (net.bSelfSym()) {
    assert(!net.hasSymNet());
    constructSelfSymNetRoutables(net);
  }
  else { // sym net
    assert(!net.bSelfSym());
    constructSymNetRoutables(net);
  }
}

void DrRoutable::constructNormalNetRoutables(Net& net) {
  Routable ro(false, net.idx());
  auto& vRoutables = net.vRoutables();
  auto& vRoutableSchedule = net.vRoutableSchedule();
  for (const auto pinIdx : net.vPinIndices()) {
    ro.vPinIndices().emplace_back(pinIdx);
  }
  vRoutables.emplace_back(ro);
  vRoutableSchedule.resize(1, 0);
}

void DrRoutable::constructSelfSymNetRoutables(Net& net) {
  // init pin shapes
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes(_cir.lef().numLayers());
  for (const auto pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vvBoxes[layerIdx].emplace_back(*cpBox);
      }
    }
  }
  for (auto& vBoxes : vvBoxes) {
    std::sort(vBoxes.begin(), vBoxes.end());
  }
  // construct routables
  auto bTotallySelfSym = [&] (const Pin& pin) -> bool {
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        Box<Int_t> symBox(*cpBox);
        symBox.flipX(_cir.symAxisX());
        if (!std::binary_search(vvBoxes[layerIdx].begin(), vvBoxes[layerIdx].end(), symBox)) {
          return false;
        }
      }
    }
    return true;
  };
  Routable roSelfSym(true, 0, net.idx());
  Routable roRest(false, 1, net.idx());
  for (const auto& pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bTotallySelfSym(pin)) {
      roSelfSym.addPinIdx(pinIdx);
    }
    else {
      roRest.addPinIdx(pinIdx);
    }
  }

  // add routables to net
  auto& vRoutables = net.vRoutables();
  auto& vRoutableSchedule = net.vRoutableSchedule();
  vRoutables.emplace_back(roSelfSym); // idx 0
  vRoutableSchedule.emplace_back(roSelfSym.idx()); // roSelfSym first
  if (roRest.numPins() > 0) {
    roRest.addRoutableIdx(roSelfSym.idx());
    vRoutables.emplace_back(roRest); // idx 1
    vRoutableSchedule.emplace_back(roRest.idx()); // roRest = roSelfSym + some pins
  }
}

void DrRoutable::constructSymNetRoutables(Net& net) {
  Net& symNet = _cir.net(net.symNetIdx());
  if (symNet.idx() < net.idx()) {
    assert(symNet.numRoutables() != 0);
    return;
  }
  // init pin shapes for both nets
  auto addPinShapes = [&] (const Net& net, Vector_t<Vector_t<Box<Int_t>>>& vvBoxes) {
    for (const auto pinIdx : net.vPinIndices()) {
      UInt_t i, layerIdx;
      const Box<Int_t>* cpBox;
      const Pin& pin = _cir.pin(pinIdx);
      Pin_ForEachLayerIdx(pin, layerIdx) {
        Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
          vvBoxes[layerIdx].emplace_back(*cpBox);
        }
      }
    }
  };
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes1(_cir.lef().numLayers());
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes2(_cir.lef().numLayers());
  addPinShapes(net, vvBoxes1);
  addPinShapes(symNet, vvBoxes2);
  for (UInt_t i = 0; i < vvBoxes1.size(); ++i) {
    std::sort(vvBoxes1[i].begin(), vvBoxes1[i].end());
    std::sort(vvBoxes2[i].begin(), vvBoxes2[i].end());
  }
  // construct routables
  auto bTotallySym = [&] (const Pin& pin, const auto& vvSymBoxes) -> bool {
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        Box<Int_t> symBox(*cpBox);
        symBox.flipX(_cir.symAxisX());
        if (!std::binary_search(vvSymBoxes[layerIdx].begin(), vvSymBoxes[layerIdx].end(), symBox)) {
          return false;
        }
      }
    }
    return true;
  };
  Routable roSym1(false, 0, net.idx(), symNet.idx(), 0);
  Routable roRest1(false, 1, net.idx());
  Routable roSym2(false, 0, symNet.idx(), net.idx(), 0);
  Routable roRest2(false, 1, symNet.idx());
  for (const auto pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bTotallySym(pin, vvBoxes2)) {
      roSym1.addPinIdx(pinIdx);
    }
    else {
      roRest1.addPinIdx(pinIdx);
    }
  }
  for (const auto pinIdx : symNet.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bTotallySym(pin, vvBoxes1)) {
      roSym2.addPinIdx(pinIdx);
    }
    else {
      roRest2.addPinIdx(pinIdx);
    }
  }
  assert(roSym1.numPins() == roSym2.numPins());
  assert(roSym1.numRoutables() == 0);
  assert(roSym1.numRoutables() == roSym2.numRoutables());
  

  // add routables to nets
  auto& vRoutables1 = net.vRoutables();
  auto& vRoutables2 = symNet.vRoutables();
  auto& vRoutableSchedule1 = net.vRoutableSchedule();
  auto& vRoutableSchedule2 = symNet.vRoutableSchedule();

  vRoutables1.emplace_back(roSym1);
  vRoutables2.emplace_back(roSym2);

  vRoutableSchedule1.emplace_back(roSym1.idx());
  vRoutableSchedule2.emplace_back(roSym2.idx());
  
  if (roRest1.numPins() > 0) {
    roRest1.addRoutableIdx(roSym1.idx());
    vRoutables1.emplace_back(roRest1);
    vRoutableSchedule1.emplace_back(roRest1.idx());
  }
  if (roRest2.numPins() > 0) {
    roRest2.addRoutableIdx(roSym2.idx());
    vRoutables2.emplace_back(roRest2);
    vRoutableSchedule2.emplace_back(roRest2.idx());
  }
}

PROJECT_NAMESPACE_END

