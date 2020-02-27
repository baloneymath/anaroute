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
    constructNetRoutables(net, net.hasSymNet(), net.bSelfSym());
    //constructNormalNetRoutables(net);
    //printNetRoutableInfo(net);
  }
}

void DrRoutable::constructNetRoutables(Net& net, const bool bSym, const bool bSelfSym) {
  if (!bSym and !bSelfSym) {
    constructNormalNetRoutables(net);
  }
  else if (bSelfSym) {
    assert(!bSym);
    constructSelfSymNetRoutables(net);
  }
  else if (bSym) { // sym net
    assert(!bSelfSym);
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
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes;
  addPinShapes(net, vvBoxes);
  
  // construct routables
  Routable roSelfSym(true);
  Routable roRest;
  roSelfSym.setNetIdx(net.idx());
  roRest.setNetIdx(net.idx());
  
  for (const auto& pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bExistTotallySymPin(pin, vvBoxes)) {
      roSelfSym.addPinIdx(pinIdx);
    }
    else {
      roRest.addPinIdx(pinIdx);
    }
  }

  // add routables to net
  auto& vRoutables = net.vRoutables();
  auto& vRoutableSchedule = net.vRoutableSchedule();
  if (roSelfSym.numPins() > 0) {
    roSelfSym.setIdx(vRoutables.size());
    vRoutables.emplace_back(roSelfSym);
    vRoutableSchedule.emplace_back(roSelfSym.idx()); // roSelfSym first
  }
  if (roRest.numPins() > 0) {
    roRest.setIdx(vRoutables.size());
    if (roRest.idx() != 0) {
      assert(roSelfSym.numPins() > 0);
      roRest.addRoutableIdx(roSelfSym.idx());
    }
    vRoutables.emplace_back(roRest);
    vRoutableSchedule.emplace_back(roRest.idx()); // roRest = roSelfSym + some pins
  }
  assert(net.numRoutables() == (Int_t)net.vRoutableSchedule().size());
}


void DrRoutable::constructSymNetRoutables(Net& net) {
  Net& symNet = _cir.net(net.symNetIdx());
  if (net.numRoutables() > 0) {
    assert(symNet.numRoutables() > 0);
    return;
  }
  // init pin shapes for both nets
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes1, vvBoxes2;
  addPinShapes(net, vvBoxes1);
  addPinShapes(symNet, vvBoxes2);
  
  // construct routables
  Routable roSym1, roRest1;
  Routable roSym2, roRest2;
  roSym1.setNetIdx(net.idx());
  roSym1.setSymNetIdx(symNet.idx());
  roRest1.setNetIdx(net.idx());
  roSym2.setNetIdx(symNet.idx());
  roSym2.setSymNetIdx(net.idx());
  roRest2.setNetIdx(symNet.idx());
  
  for (const auto pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bExistTotallySymPin(pin, vvBoxes2)) {
      roSym1.addPinIdx(pinIdx);
    }
    else {
      roRest1.addPinIdx(pinIdx);
    }
  }
  for (const auto pinIdx : symNet.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bExistTotallySymPin(pin, vvBoxes1)) {
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
  
  if (roSym1.numPins() > 0) {
    assert(roSym2.numPins() > 0);
    roSym1.setIdx(vRoutables1.size());
    roSym2.setIdx(vRoutables2.size());
    roSym1.setSymNetRoutableIdx(roSym2.idx());
    roSym2.setSymNetRoutableIdx(roSym1.idx());
    vRoutables1.emplace_back(roSym1);
    vRoutables2.emplace_back(roSym2);
    vRoutableSchedule1.emplace_back(roSym1.idx());
    vRoutableSchedule2.emplace_back(roSym2.idx());
  }
  
  if (roRest1.numPins() > 0) {
    roRest1.setIdx(vRoutables1.size());
    if (roRest1.idx() != 0) {
      assert(roSym1.numPins() > 0);
      roRest1.addRoutableIdx(roSym1.idx());
    }
    vRoutables1.emplace_back(roRest1);
    vRoutableSchedule1.emplace_back(roRest1.idx());
  }
  if (roRest2.numPins() > 0) {
    roRest2.setIdx(vRoutables2.size());
    if (roRest2.idx() != 0) {
      assert(roSym2.numPins() > 0);
      roRest2.addRoutableIdx(roSym2.idx());
    }
    vRoutables2.emplace_back(roRest2);
    vRoutableSchedule2.emplace_back(roRest2.idx());
  }
}


bool DrRoutable::bCrossSymNet(const Net& net) {
  if (!net.hasSymNet())
    return false;
  const Net& symNet = _cir.net(net.symNetIdx());
  return bCrossSymAxisX(net) and bCrossSymAxisX(symNet);
}

bool DrRoutable::bCrossSymAxisX(const Net& net) {
  bool bHasLeft = false, bHasRight = false;
  UInt_t i, j, pinIdx, layerIdx;
  const Box<Int_t>* cpBox;
  Net_ForEachPinIdx(net, pinIdx, i) {
    const auto& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, j) {
        if (cpBox->xh() < net.symAxisX())
          bHasLeft = true;
        if (cpBox->xl() > net.symAxisX())
          bHasRight = true;
      }
    }
  }
  return bHasLeft & bHasRight;
}

void DrRoutable::constructCrossSymNetRoutables(Net& net) {
  Net& symNet = _cir.net(net.symNetIdx());
  if (net.numRoutables() > 0) {
    assert(symNet.numRoutables() > 0);
    return;
  }
  // init pin shapes for both nets
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes1;
  Vector_t<Vector_t<Box<Int_t>>> vvBoxes2;
  addPinShapes(net, vvBoxes1);
  addPinShapes(symNet, vvBoxes2);
  
  // construct routables
  std::array<Routable, 2> roSym1, roSym2; // 0: left, 1: right
  Routable roRest1, roRest2;
  for (auto& ro : roSym1) {
    ro.setNetIdx(net.idx());
    ro.setSymNetIdx(symNet.idx());
  }
  for (auto& ro : roSym2) {
    ro.setNetIdx(symNet.idx());
    ro.setSymNetIdx(net.idx());
  }
  roRest1.setNetIdx(net.idx());
  roRest2.setNetIdx(symNet.idx());
  
  for (const auto pinIdx : net.vPinIndices()) {
    const Pin& pin = _cir.pin(pinIdx);
    if (bExistTotallySymPin(pin, vvBoxes2)) {
      if (bPinOnLeft(pin))
        roSym1[0].addPinIdx(pinIdx);
      else
        roSym1[1].addPinIdx(pinIdx);
    }
    else {
      roRest1.addPinIdx(pinIdx);
    }
  }
  // TODO
}

void DrRoutable::addPinShapes(const Net& net, Vector_t<Vector_t<Box<Int_t>>>& vvBoxes) {
  vvBoxes.resize(_cir.lef().numLayers());
  for (const Int_t pinIdx : net.vPinIndices()) {
    UInt_t i, layerIdx;
    const Box<Int_t>* cpBox;
    const Pin& pin = _cir.pin(pinIdx);
    Pin_ForEachLayerIdx(pin, layerIdx) {
      Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
        vvBoxes[layerIdx].emplace_back(*cpBox);
      }
    }
  }
  for (auto& vBoxes : vvBoxes) {
    std::sort(vBoxes.begin(), vBoxes.end());
  }
}

bool DrRoutable::bExistTotallySymPin(const Pin& pin, const Vector_t<Vector_t<Box<Int_t>>>& vvSymBoxes) {
  const Net& net = _cir.net(pin.netIdx());
  UInt_t i, layerIdx;
  const Box<Int_t>* cpBox;
  Pin_ForEachLayerIdx(pin, layerIdx) {
    Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
      Box<Int_t> symBox(*cpBox);
      symBox.flipX(net.symAxisX());
      if (!std::binary_search(vvSymBoxes[layerIdx].begin(), vvSymBoxes[layerIdx].end(), symBox)) {
        return false;
      }
    }
  }
  return true;
}

bool DrRoutable::bPinOnLeft(const Pin& pin) {
  const Net& net = _cir.net(pin.netIdx());
  UInt_t i, layerIdx;
  const Box<Int_t>* cpBox;
  Pin_ForEachLayerIdx(pin, layerIdx) {
    Pin_ForEachLayerBox(pin, layerIdx, cpBox, i) {
      if (cpBox->xh() >= net.symAxisX())
        return false;
    }
  }
  return true;
}

void DrRoutable::printNetRoutableInfo(const Net& net) {
  cerr << "Net: " << net.name() << endl;
  cerr << "Pins: ";
  for (auto pinIdx : net.vPinIndices())
    cerr << pinIdx << " ";
  cerr << endl;
  for (Int_t j = 0; j < net.numRoutables(); ++j) {
    cerr << "  Routable: " << j << endl;
    cerr << "    Pins: ";
    for (Int_t k = 0; k < net.routable(j).numPins(); ++k)
      cerr << net.routable(j).pinIdx(k) << " ";
    cerr << endl;
    cerr << "    Routables: ";
    for (Int_t k = 0; k < net.routable(j).numRoutables(); ++k)
      cerr << net.routable(j).routableIdx(k) << " ";
    cerr << endl;
  }
  cerr << "  Schedule: ";
  for (Int_t j = 0; j < net.numRoutables(); ++j) {
    cerr << net.routableSchedule(j) << " ";
  }
  cerr << endl << endl;

}

PROJECT_NAMESPACE_END

