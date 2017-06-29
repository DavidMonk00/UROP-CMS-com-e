/**
  I2CRegister.cpp
  Purpose: defines functions for the derived I2CBaseRegister classes.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CRegister.h"

/**
  Class constructor.
  @param addr - address of register within I2C device
  @param read_func - lambda function to convert hexadecimal value into correct
                     units type
  @param write_func - lambda function to convert units type into uint8_t to be
                      to be written
*/
GenericI2CRegister::GenericI2CRegister(uint32_t addr, std::function<units_variant(double)> read_func,
                                                      std::function<uint8_t(units_variant)> write_func) {
  address = addr;
  mRead = read_func;
  mWrite = write_func;
}

/**
   Class destructor.
*/
GenericI2CRegister::~GenericI2CRegister(void) {}

/**
  Read data from register.
  @param i2c_ptr - pointer to I2C class used for transport
  @return units_variant containing quantity of correct type.
*/
units_variant GenericI2CRegister::read(I2C_base* i2c_ptr) {
  char buffer = 0;
  i2c_ptr->receiveData(&buffer, 1, address);
  return mRead((double)buffer);
}

/**
  Write data to register.
  @param i2c_ptr - pointer to I2C class used for transport
  @param value - data to be written. Must be of correct type
*/
void GenericI2CRegister::write(I2C_base* i2c_ptr, units_variant value) {
  char buffer = mWrite(value);
  i2c_ptr->sendData(&buffer, 1, address);
}

/**
  Class constructor.
  @param addr - address of register within I2C device
  @param read_func - lambda function to convert hexadecimal value into correct
                     units type
  @param write_func - lambda function to convert units type into uint8_t to be
                      to be written
*/
TimeI2CRegister::TimeI2CRegister(uint32_t addr, std::function<units_variant(double)> read_func,
                                                std::function<uint8_t(units_variant)> write_func) {
  address = addr;
  mRead = read_func;
  mWrite = write_func;
}

/**
   Class destructor.
*/
TimeI2CRegister::~TimeI2CRegister(void) {}

/**
  Read data from register.
  @param i2c_ptr - pointer to I2C class used for transport
  @return units_variant containing quantity of correct type.
*/
units_variant TimeI2CRegister::read(I2C_base* i2c_ptr) {
  char buffer = 0;
  i2c_ptr->receiveData(&buffer, 1, address);
  double t = 10*(buffer/16) + buffer%16;
  return mRead(t);
}

/**
  Write data to register.
  @param i2c_ptr - pointer to I2C class used for transport
  @param value - data to be written. Must be of correct type
*/
void TimeI2CRegister::write(I2C_base* i2c_ptr, units_variant value) {
  char buffer = (char)boost::get<quantity<si::time> >(value).value();
  i2c_ptr->sendData(&buffer, 1, address);
}
