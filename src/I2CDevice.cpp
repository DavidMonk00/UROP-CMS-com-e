/**
  @file I2CDevice.cpp
  @brief Defines functions for the I2CDevice class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CDevice.h"

/**
  @brief Class constructor.
  @param addr - Address of device.
  @param reg_map - Unordered map of registers available on device.
*/
I2CDevice::I2CDevice(uint32_t addr, std::unordered_map<std::string, I2CBaseRegister*> reg_map) {
  registers = reg_map;
  address = addr;
}

/**
   @brief Default class constructor.
*/
I2CDevice::I2CDevice(void) {}

void I2CDevice::setI2CType(I2C_base* i2c_type) {
   i2c = i2c_type;
}

/**
   @brief Class destructor.
*/
I2CDevice::~I2CDevice(void) {}

/**
  @brief Read data from register.
  @param reg - Property to read.
  @return units_variant containing quantity of correct type.
*/
units_variant I2CDevice::read(std::string reg) {
  i2c_reg = registers[reg];
  return i2c_reg->read(i2c, address);
}

/**
  @brief Write data to register.
  @param reg - Property to write to.
  @param value - Data to write to register. Must be of the correct type.
*/
void I2CDevice::write(std::string reg, units_variant value) {
  i2c_reg = registers[reg];
  i2c_reg->write(i2c, address, value);
}

/**
  @brief Get all possible registers for given device.
  @return Vector of strings assosciated with the properties.
*/
std::vector<std::string>I2CDevice::getProperties(void) {
  std::vector<std::string> v;
  v.reserve(registers.size());
  for (auto kv : registers) {
    v.push_back(kv.first);
  }
  return v;
}
