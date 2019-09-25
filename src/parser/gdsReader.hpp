/**
 * @file   gdsReader.hpp
 * @brief  Parser - Gds format parser
 * @author Hao Chen
 * @date   09/23/2019
 *
 **/

#ifndef _GDS_READER_HPP_
#define _GDS_READER_HPP_

#include <limbo/parsers/gdsii/gdsdb/GdsIO.h>
#include <limbo/parsers/gdsii/gdsdb/GdsObjectHelpers.h>

#include "src/global/global.hpp"
#include "src/db/dbCir.hpp"

PROJECT_NAMESPACE_START

class GdsReader { // from limbo
 public:
  GdsReader(CirDB& c)
    : _cir(c) {}
  ~GdsReader() {}
  
  void parse(const String_t& filename);

 private:
  CirDB&                            _cir;
  Int_t                             _scale;
  Vector_t<Pair_t<Int_t, Index_t>>  _vMaskId2Layer;

  /////////////////////////////////////////
  //    Private functions                //
  /////////////////////////////////////////
  String_t  topCell(GdsParser::GdsDB::GdsDB db);
  void      buildLayerMap();
  /////////////////////////////////////////
  //    Helper functions                 //
  /////////////////////////////////////////
  Int_t     to_db_unit(const Int_t n) const;

};
//////////////////////////////////////////////////////////////////


namespace GetSRefNameActionDetailsParser {
	/// @brief default type
	template<typename ObjectType>
	inline void getSName(std::string &name,  ::GdsParser::GdsRecords::EnumType type, ObjectType *object) {}

	/// @brief SREF type
	template<>
	inline void getSName(std::string &name,  ::GdsParser::GdsRecords::EnumType type, ::GdsParser::GdsDB::GdsCellReference *object) {
		name = object->refCell();
	}
}

/// @brief aution function object to get the cell reference name
struct GetSRefNameActionParser {
  /// @param A reference to the string to record the name of the sref
  GetSRefNameActionParser(std::string &name) : _name(name) {}
  template<typename ObjectType>
  void operator()(::GdsParser::GdsRecords::EnumType type, ObjectType* object) {
    GetSRefNameActionDetailsParser::getSName(_name, type, object);
  }

  /// @return a message of action for debug
  std::string message() const  {
    return "GetSRefNameAction";
  }


  std::string &_name; ///< The cell reference name
};

PROJECT_NAMESPACE_END

#endif /// _GDS_READER_HPP_
