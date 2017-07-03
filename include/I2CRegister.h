/**
  I2CRegister.h
  Purpose: defines I2CBaseRegister class and its derivations.
  @author David Monk
  @version 1.0
*/

#include <string>
#include <cstdlib>
#include <unordered_map>
#include <stdlib.h>
#include <functional>
#include <stdio.h>
#include "I2C_base.h"
#include "units_define.h"

//TODO Add minutes, hours to units namespace

/**
  Base class for I2C register is pure virtual and thus should not be constructed.
*/
class I2CBaseRegister {
public:
   virtual units_variant read(I2C_base* i2c_ptr) = 0;
   virtual void write(I2C_base* i2c_ptr, units_variant value) = 0;
};

/**
  Derived I2CBaseRegister class for generic calls through lambda functions. Used
  when the interface is simple.
*/
class GenericI2CRegister : public I2CBaseRegister {
   uint32_t address;
   std::function<units_variant(double value)> mRead;
   std::function<uint8_t(units_variant)> mWrite;
public:
   GenericI2CRegister(uint32_t addr, std::function<units_variant(double value)> read_func,
                                     std::function<uint8_t(units_variant)> write_func);
   ~GenericI2CRegister(void);
   units_variant read(I2C_base* i2c_ptr);
   void write(I2C_base* i2c_ptr, units_variant value);
};

/**
  Derived I2CBaseRegister class for calls to timing registers. Formats the output
  correctly when it is given in pure hexadecimal 0-59.
*/
class TimeI2CRegister : public I2CBaseRegister {
  uint32_t address;
  std::function<units_variant(double)> mRead;
  std::function<uint8_t(units_variant)> mWrite;
public:
  TimeI2CRegister(uint32_t addr, std::function<units_variant(double)> read_func,
                                 std::function<uint8_t(units_variant)> write_func);
  ~TimeI2CRegister(void);
  units_variant read(I2C_base* i2c_ptr);
  void write(I2C_base* i2c_ptr, units_variant value);
};

/**
  Derived I2CBaseRegister class for calls to DS3232 temperature register.
*/
class DS3232TemperatureI2CRegister : public I2CBaseRegister {
  uint32_t address;
  std::function<units_variant(double)> mRead;
  std::function<uint8_t(units_variant)> mWrite;
public:
  DS3232TemperatureI2CRegister(uint32_t addr, std::function<units_variant(double)> read_func,
                                 std::function<uint8_t(units_variant)> write_func);
  ~DS3232TemperatureI2CRegister(void);
  units_variant read(I2C_base* i2c_ptr);
  void write(I2C_base* i2c_ptr, units_variant value);
};
