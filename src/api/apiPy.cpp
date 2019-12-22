/**
 * @file   apiPy.cpp
 * @brief  API for python
 * @author Hao Chen
 * @date   12/22/2019
 *
 **/

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"
#include "src/parser/parser.hpp"
#include "src/gr/grMgr.hpp"
#include "src/ta/taMgr.hpp"
#include "src/dr/drMgr.hpp"
#include "src/drc/drcMgr.hpp"
#include "src/writer/writer.hpp"

namespace py = pybind11;
namespace pro = PROJECT_NAMESPACE;

namespace apiPy {
  class AnaroutePy {
   public:
    AnaroutePy() : _par(_cir), _wr(_cir) {}
    ~AnaroutePy() {}

    // parse
    void parseLef(const pro::String_t& filename) { _par.parseLef(filename); }
    void parseTechfile(const pro::String_t& filename) { _par.parseTechfile(filename); }
    void parseGds(const pro::String_t& filename) { _par.parseGds(filename); }
    void parseIspd08(const pro::String_t& filename) { _par.parseIspd08(filename); }
    void parseSymNet(const pro::String_t& filename) { _par.parseSymNet(filename); }
    void parseIOPin(const pro::String_t& filename) { _par.parseIOPin(filename); }

    // solve
    void solve() {
      _par.correctPinNBlkLoc();
      _cir.buildSpatial();
      _cir.markBlks();
      pro::GrMgr gr(_cir);
      gr.solve();
      _cir.buildSpatialNetGuides();
      pro::TaMgr ta(_cir);
      ta.solve();
      pro::DrcMgr drc(_cir);
      pro::DrMgr dr(_cir, drc);
      dr.solve();
    }

    // write
    void writeLayoutGds(const pro::String_t& placefilename, const pro::String_t& outfilename, const bool bFlatten) {
      _wr.writeLayoutGds(placefilename, outfilename, bFlatten);
    }
    void writeDumb(const pro::String_t& placefilename, const pro::String_t& outfilename) {
      _wr.writeDumb(placefilename, outfilename);
    }

   private:
    pro::CirDB  _cir;
    pro::Parser _par;
    pro::Writer _wr;
  };
}

void initPyAPI(py::module& m) {
  py::class_<apiPy::AnaroutePy>(m, "AnaroutePy")
    .def(py::init<>())
    .def("parseLef", &apiPy::AnaroutePy::parseLef)
    .def("parseTechfile", &apiPy::AnaroutePy::parseTechfile)
    .def("parseGds", &apiPy::AnaroutePy::parseGds)
    .def("parseIspd08", &apiPy::AnaroutePy::parseIspd08)
    .def("parseSymNet", &apiPy::AnaroutePy::parseSymNet)
    .def("parseIOPin", &apiPy::AnaroutePy::parseIOPin)
    .def("solve", &apiPy::AnaroutePy::solve)
    .def("writeLayoutGds", &apiPy::AnaroutePy::writeLayoutGds)
    .def("writeDumb", &apiPy::AnaroutePy::writeDumb);

}

PYBIND11_MAKE_OPAQUE(std::vector<PROJECT_NAMESPACE::UInt_t>);

PYBIND11_MODULE(anaroutePy, m) {
  initPyAPI(m);
}
