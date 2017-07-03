/**
  I2CDevice.cpp
  Purpose: defines functions for the I2CDevice class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CDevice.h"
extern register_map registers::DS3232;

/**
  Class constructor.
  @param i2c_if pointer to I2C class used for transport.
*/
I2CDevice::I2CDevice(I2C_base* i2c_if) {
  i2c = i2c_if;
  registers = registers::DS3232;
}

/**
   Class destructor.
*/
I2CDevice::~I2CDevice(void) {}

/**
  Read data from register.
  @param reg - property to read
  @return units_variant containing quantity of correct type.
*/
units_variant I2CDevice::read(std::string reg) {
  i2c_reg = registers[reg];
  I2C_base* ptr;
  return i2c_reg->read(i2c);
}

/**
  Write data to register.
  @param reg - property to write to
  @param value - data to write to register. Must be of the correct type
*/
void I2CDevice::write(std::string reg, units_variant value) {
  i2c_reg = registers[reg];
  I2C_base* ptr;
  i2c_reg->write(i2c, value);
}
