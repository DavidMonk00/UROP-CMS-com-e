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
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/electric_potential.hpp>
#include <boost/units/systems/si/current.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/variant.hpp>
#include "I2CRegister.h"
#include "device_maps.h"
#include "I2C_base.h"

using namespace boost::units;
using namespace boost::units::si;

/**
  Class represents a single I2C device with a number of registers. Read/Write
  functions take a string-based ID for a given register.
*/
class I2CDevice {
private:
  register_map registers;
  I2CBaseRegister* i2c_reg;
  I2C_base* i2c;
public:
  I2CDevice(I2C_base* i2c_if);
  ~I2CDevice(void);
  units_variant read(std::string reg);
  void write(std::string reg, units_variant value);
};
