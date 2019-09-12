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
    : _hasDatabase(false), _hasCapacitance(false), _hasResistance(false), _hasTime(false),
      _hasPower(false), _hasCurrent(false), _hasVoltage(false), _hasFrequency(false),
      _databaseName(""), _databaseNumber(0), _capacitance(0), _resistance(0),
      _time(0), _power(0), _current(0), _voltage(0), _frequency(0) {}
  ~LefUnits() {}
  
  /////////////////////////////////
  //    Getter                   //
  /////////////////////////////////
  bool hasDatabase()    const { return _hasDatabase; }
  bool hasCapacitance() const { return _hasCapacitance; }
  bool hasResistance()  const { return _hasResistance; }
  bool hasTime()        const { return _hasTime; }
  bool hasPower()       const { return _hasPower; }
  bool hasCurrent()     const { return _hasCurrent; }
  bool hasVoltage()     const { return _hasVoltage; }
  bool hasFrequency()   const { return _hasFrequency; }

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
  bool _hasDatabase;
  bool _hasCapacitance;
  bool _hasResistance;
  bool _hasTime;
  bool _hasPower;
  bool _hasCurrent;
  bool _hasVoltage;
  bool _hasFrequency;

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
  
  void setDatabaseName(const String_t& s)     { _databaseName = s; _hasDatabase = true; }
  void setDatabaseNumber(const Real_t n)      { _databaseNumber= n; _hasDatabase = true; }
  void setCapacitance(const Real_t c)         { _capacitance = c; _hasCapacitance = true; }
  void setResistance(const Real_t r)          { _resistance = r; _hasResistance = true; }
  void setTime(const Real_t t)                { _time = t; _hasTime = true; }
  void setPower(const Real_t p)               { _power = p; _hasPower = true; }
  void setCurrent(const Real_t c)             { _current = c; _hasCurrent = true; }
  void setVoltage(const Real_t v)             { _voltage = v; _hasVoltage = true; }
  void setFrequency(const Real_t f)           { _frequency = f; _hasFrequency = true; }
};


PROJECT_NAMESPACE_END

#endif /// _DB_LEF_UNITS_HPP_

