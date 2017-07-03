/**
  I2CDevice.cpp
  Purpose: defines functions for the I2CDevice class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CDevice.h"

register_map DS3232_reg = {{"seconds",     new TimeI2CRegister(0x00, [](double t){return t*si::seconds;},
                                                                     [](units_variant value) {return (uint8_t)boost::get<quantity<si::time> >(value).value();})},
                           {"temperature", new DS3232TemperatureI2CRegister(0x11, [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})},
                           {"SRAM0",       new GenericI2CRegister(0x14, [](double value){return value;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<double>(value);})},
                           {"SRAM1",       new GenericI2CRegister(0x14, [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})}};


/**
  Class constructor.
  @param i2c_if pointer to I2C class used for transport.
*/
I2CDevice::I2CDevice(I2C_base* i2c_if) {
  i2c = i2c_if;
  registers = DS3232_reg;
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
