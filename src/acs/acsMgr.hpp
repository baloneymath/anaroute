/**
 * @file   acsMgr.hpp
 * @brief  Access points manager
 * @author Keren Zhu
 * @date   02/05/2020
 *
 **/

#ifndef ANAROUTE_ACS_MGR_HPP_
#define ANAROUTE_ACS_MGR_HPP_

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START


/// @brief Manage the generation of the access points
class AcsMgr {
 public:
  explicit AcsMgr(CirDB & c)
    : _cir(c) {}
  /// @brief compute the access points and push the results into the circuit db
  void computeAcs();
  /// @brief compute the access points for one pin
  /// @param the pin index
  void computePinAcs(const UInt_t pinIdx);
  void computeBoxAcs(const Box<Int_t>& box, const Int_t layerIdx, Vector_t<Point3d<Int_t>>& vAcs);
  /// @brief if using one candidate of access point
  /// @param pin index
  /// @param a 3d point representing the grid indices
  /// @param a unsign int representing the box index in the pin
  /// @return true: use it. false: prune it 
  bool checkAc(const UInt_t pinIdx, const Point3d<Int_t>& gridPt, const UInt_t boxIdx) const;

 private:
  CirDB&  _cir;
};

PROJECT_NAMESPACE_END

#endif //ANAROUTE_ACS_MGR_HPP_
