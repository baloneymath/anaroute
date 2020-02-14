/**
 * @file   lefVia.hpp
 * @brief  Technology configuration - Lef Via
 * @author Hao Chen, Keren Zhu
 * @date   09/16/2019
 *
 **/

#ifndef _DB_LEF_VIA_HPP_
#define _DB_LEF_VIA_HPP_

#include "src/global/global.hpp"
#include "src/geo/box.hpp"
#include "src/util/Vector2D.h"

PROJECT_NAMESPACE_START

class ViaGenerationNotImplementedException : public std::exception 
{
    public:
    virtual const char* what() const throw()
    {
        return "Generation for the given LefViaRule type has not been implemented";
    }
};

class ViaGenerationInfeasibleException : public std::exception
{
    public:
    virtual const char* what() const throw()
    {
        return "Infeasible VIA generation from ViaRule";
    }
};

class ViaGenerationInfeasibleMaxWidthException : public ViaGenerationInfeasibleException
{
    virtual const char* what() const throw()
    {
        return "Infeasible VIA generation from ViaRule: exceed max metal width";
    }
};

class LefDB;

class LefVia {
  friend class LefReader;
  friend class LefViaTable;
 public:
  LefVia()
    : _name(""), _bDefault(false), _resistance(0),
      _layerIndices{0, 0 , 0}, _layerNames{"", "", ""} {}
  ~LefVia() {}

  /////////////////////////////////
  //    Getter                   //
  /////////////////////////////////
  const String_t&               name()                  const { return _name; }
  bool                          bDefault()              const { return _bDefault; }
  Int_t                         resistance()            const { return _resistance; }
  UInt_t                        botLayerIdx()           const { return _layerIndices[0]; }
  UInt_t                        cutLayerIdx()           const { return _layerIndices[1]; }
  UInt_t                        topLayerIdx()           const { return _layerIndices[2]; }
  const String_t&               botLayerName()          const { return _layerNames[0]; } 
  const String_t&               cutLayerName()          const { return _layerNames[1]; } 
  const String_t&               topLayerName()          const { return _layerNames[2]; }
  
  // Boxes
  UInt_t                        numBotBoxes()           const { return _vBoxes[0].size(); }
  UInt_t                        numCutBoxes()           const { return _vBoxes[1].size(); }
  UInt_t                        numTopBoxes()           const { return _vBoxes[2].size(); }
  const Box<Int_t>&             botBox(const UInt_t i)  const { return _vBoxes[0][i]; }
  const Box<Int_t>&             cutBox(const UInt_t i)  const { return _vBoxes[1][i]; }
  const Box<Int_t>&             topBox(const UInt_t i)  const { return _vBoxes[2][i]; }
  const Vector_t<Box<Int_t>>&   vBotBoxes()             const { return _vBoxes[0]; }
  const Vector_t<Box<Int_t>>&   vCutBoxes()             const { return _vBoxes[1]; }
  const Vector_t<Box<Int_t>>&   vTopBoxes()             const { return _vBoxes[2]; }
  bool bValid() const { return _valid; }



                                         
  // for debug
  void logInfo() const;
 protected:                                                        
  String_t             _name;
  bool                 _bDefault; // true: fixed via, false: generated via
  Int_t                _resistance;
  UInt_t               _layerIndices[3]; // 0 -> botLayer 1 -> cutLayer 2 -> topLayer
  String_t             _layerNames[3];   // 0 -> botLayer 1 -> cutLayer 2 -> topLayer 
  Vector_t<Box<Int_t>> _vBoxes[3];       // 0 -> botLayer 1 -> cutLayer 2 -> topLayer 
  //Box<Int_t>           _cutBBox = Box<Int_t> (MAX_INT, MAX_INT, MIN_INT, MIN_INT); ///< The bounding box for the cut layer
  bool                 _valid = false;

 public:
  /////////////////////////////////
  //    Setter                   //
  /////////////////////////////////
  void setName(const String_t& n);
  void setDefault(bool bDefault) { _bDefault = bDefault; }
  void setResistance(const Int_t r);
  void setLayerIdx(const UInt_t i, const UInt_t v);
  void setLayerName(const UInt_t i, const String_t& n);
  void addBox(const UInt_t i, const Box<Int_t>& b);
  void setValid(bool valid) { _valid = valid; }
};

/*
class LefViaImplementorBase;

class LefViaPrototype : public LefVia
{
  protected:
    std::unique_ptr<LefViaImplementorBase> _implementor = nullptr;
};
*/

/// @brief the table for LefVia
class LefViaTable
{
    protected:
        static constexpr UInt_t MAX_GENERATE_ROW = 6; ///< The max number of rows to generate
        static constexpr UInt_t MIN_GENERATE_ROW = 1; ///< The min number of rows to generate
        static constexpr UInt_t MAX_GENERATE_COLUMN = 6; ///< The max number of columns to generate
        static constexpr UInt_t MIN_GENERATE_COLUMN = 1; ///< The min number of columns to generate
    public:
        LefViaTable() 
        {
            _table.clear();
        }
        /// @brief constructor with number of layers
        /// @param the number of layers need to consider
        explicit LefViaTable(UInt_t numLayers) 
        {
            _table.resize(numLayers, 
                    Vector2D<LefVia>(MAX_GENERATE_ROW - MIN_GENERATE_ROW + 1,
                                     MAX_GENERATE_COLUMN - MIN_GENERATE_COLUMN +1)
                    ); // size: # of layers * # of rows * # of columns
        }
        /// @brief genearte the vias based on viarules
        /// @param the LEF tech database
        void generateVias(LefDB &lef);
        LefVia & via(UInt_t lowerMetalLayer, UInt_t row, UInt_t col)
        {
            return _table.at(lowerMetalLayer).at(row - MIN_GENERATE_ROW, col - MIN_GENERATE_COLUMN);
        }
        const LefVia & via(UInt_t lowerMetalLayer, UInt_t row, UInt_t col) const
        {
            return _table.at(lowerMetalLayer).at(row - MIN_GENERATE_ROW, col - MIN_GENERATE_COLUMN);
        }
    private:
        Vector_t<Vector2D<LefVia>> _table; ///< _table[layer][# of rows][# of columns] = via
};

PROJECT_NAMESPACE_END

#endif /// _DB_LEF_VIA_HPP_
