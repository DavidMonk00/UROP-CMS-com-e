/**
  I2CDevice.cpp
  Purpose: defines functions for the I2CDevice class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CDevice.h"

/**
  Class constructor.
  @param i2c_if pointer to I2C class used for transport.
*/
I2CDevice::I2CDevice(uint32_t addr, std::unordered_map<std::string, I2CBaseRegister*> reg_map) {
  registers = reg_map;
  address = addr;
}

I2CDevice::I2CDevice(void) {}

void I2CDevice::setI2CType(tI2Ctype type) {
   switch (type) {
      case tI2Ctype::SEMA:
         i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
         break;
      default:
         std::cout << "Something else" << '\n';
   }
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
  return i2c_reg->read(i2c, address);
}

/**
  Write data to register.
  @param reg - property to write to
  @param value - data to write to register. Must be of the correct type
*/
void I2CDevice::write(std::string reg, units_variant value) {
  i2c_reg = registers[reg];
  i2c_reg->write(i2c, address, value);
}

/**
  Get all possible registers for given device.
  @return vector of strings assosciated with the properties.
*/
std::vector<std::string>I2CDevice::getProperties(void) {
  std::vector<std::string> v;
  v.reserve(registers.size());
  for (auto kv : registers) {
    v.push_back(kv.first);
  }
  return v;
}
