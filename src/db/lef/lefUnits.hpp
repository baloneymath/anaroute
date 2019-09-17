/**
 * @file   lefUnits.hpp
 * @brief  Technology configuration - Lef Units
 * @author Hao Chen
 * @date   09/11/2019
 *
 **/

#ifndef _DB_LEF_UNITS_HPP_
#define _DB_LEF_UNITS_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START

class LefUnits {
  friend class LefReader;

 public:
  LefUnits()
    : _databaseName(""),
      _databaseNumber(1e2),
      _capacitance(1e6),
      _resistance(1e4),
      _time(1e3),
      _power(1e4),
      _current(1e4),
      _voltage(1e3),
      _frequency(1) {}
  ~LefUnits() {}
  
  /////////////////////////////////
  //    Getter                   //
  /////////////////////////////////
  const String_t&    databaseName()    const { return _databaseName; }
  Real_t             databaseNumber()  const { return _databaseNumber; }
  Real_t             capacitance()     const { return _capacitance; }
  Real_t             resistance()      const { return _resistance; }
  Real_t             time()            const { return _time; }
  Real_t             power()           const { return _power; }
  Real_t             current()         const { return _current; }
  Real_t             voltage()         const { return _voltage; }
  Real_t             frequency()       const { return _frequency; }

 private:
  String_t    _databaseName;
  Real_t      _databaseNumber;
  Real_t      _capacitance;
  Real_t      _resistance;
  Real_t      _time;
  Real_t      _power;
  Real_t      _current;
  Real_t      _voltage;
  Real_t      _frequency;
  
  /////////////////////////////////
  //    Setter                   //
  /////////////////////////////////
  void setDatabaseName(const String_t& s)     { _databaseName = s; }
  void setDatabaseNumber(const Real_t n)      { _databaseNumber= n; }
  void setCapacitance(const Real_t c)         { _capacitance = c; }
  void setResistance(const Real_t r)          { _resistance = r; }
  void setTime(const Real_t t)                { _time = t; }
  void setPower(const Real_t p)               { _power = p; }
  void setCurrent(const Real_t c)             { _current = c; }
  void setVoltage(const Real_t v)             { _voltage = v; }
  void setFrequency(const Real_t f)           { _frequency = f; }
};


PROJECT_NAMESPACE_END

#endif /// _DB_LEF_UNITS_HPP_

