/**
  I2CDevice.h
  Purpose: defines I2CDevice class.
  @author David Monk
  @version 1.0
*/

#pragma once
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <stdlib.h>
#include <functional>
#include <stdio.h>
//#include <boost/units/io.hpp>
#include <boost/variant.hpp>
#include "I2CRegister.h"
#include "I2C_base.h"

using namespace boost::units;
using namespace boost::units::si;

typedef std::unordered_map<std::string, I2CBaseRegister*> register_map;

/**
  Class represents a single I2C device with a number of registers. Read/Write
  functions take a string-based ID for a given register.
*/
class I2CDevice {
private:
   uint32_t address;
   std::unordered_map<std::string, I2CBaseRegister*> registers;
   I2CBaseRegister* i2c_reg;
   I2C_base* i2c;
public:
   enum class tI2Ctype {SEMA,FTDI,PCIe};
   I2CDevice(uint32_t addr, std::unordered_map<std::string, I2CBaseRegister*> reg_map);
   I2CDevice(void);
   ~I2CDevice(void);
   void setI2CType(tI2Ctype type);
   units_variant read(std::string reg);
   void write(std::string reg, units_variant value);
   std::vector<std::string> getProperties(void);
};
