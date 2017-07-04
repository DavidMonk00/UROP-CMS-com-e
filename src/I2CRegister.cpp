/**
  I2CRegister.cpp
  Purpose: defines functions for the derived I2CBaseRegister classes.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CRegister.h"

/**
  Class constructor.
  @param r - reg of register within I2C device
  @param read_func - lambda function to convert hexadecimal value into correct
                     units type
  @param write_func - lambda function to convert units type into uint8_t to be
                      to be written
*/
GenericI2CRegister::GenericI2CRegister(uint32_t r, std::function<units_variant(double)> read_func,
                                                      std::function<uint8_t(units_variant)> write_func) {
  reg = r;
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
units_variant GenericI2CRegister::read(I2C_base* i2c_ptr, uint32_t address) {
  char buffer = 0;
  i2c_ptr->receiveData(address, &buffer, 1, reg);
  return mRead((double)buffer);
}

/**
  Write data to register.
  @param i2c_ptr - pointer to I2C class used for transport
  @param value - data to be written. Must be of correct type
*/
void GenericI2CRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
  char buffer = mWrite(value);
  i2c_ptr->sendData(address, &buffer, 1, reg);
}

/**
  Class constructor.
  @param r - reg of register within I2C device
  @param read_func - lambda function to convert hexadecimal value into correct
                     units type
  @param write_func - lambda function to convert units type into uint8_t to be
                      to be written
*/
TimeI2CRegister::TimeI2CRegister(uint32_t r, std::function<units_variant(double)> read_func,
                                                std::function<uint8_t(units_variant)> write_func) {
  reg = r;
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
units_variant TimeI2CRegister::read(I2C_base* i2c_ptr, uint32_t address) {
  char buffer = 0;
  i2c_ptr->receiveData(address, &buffer, 1, reg);
  double t = 10*(buffer/16) + buffer%16;
  return mRead(t);
}

/**
  Write data to register.
  @param i2c_ptr - pointer to I2C class used for transport
  @param value - data to be written. Must be of correct type
*/
void TimeI2CRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
  char buffer = (char)mWrite(value);
  i2c_ptr->sendData(address, &buffer, 1, reg);
}

/**
  Class constructor.
  @param r - reg of register within I2C device
  @param read_func - lambda function to convert hexadecimal value into correct
                     units type
  @param write_func - lambda function to convert units type into uint8_t to be
                      to be written
*/
DS3232TemperatureI2CRegister::DS3232TemperatureI2CRegister(uint32_t r, std::function<units_variant(double)> read_func,
                                                                          std::function<uint8_t(units_variant)> write_func) {
  reg = r;
  mRead = read_func;
  mWrite = write_func;
}

/**
   Class destructor.
*/
DS3232TemperatureI2CRegister::~DS3232TemperatureI2CRegister(void) {}

/**
  Read data from register.
  @param i2c_ptr - pointer to I2C class used for transport
  @return units_variant containing quantity of correct type.
*/
units_variant DS3232TemperatureI2CRegister::read(I2C_base* i2c_ptr, uint32_t address) {
  char* buffer = (char*)malloc(2*sizeof(char));
  i2c_ptr->receiveData(address, buffer, 2, reg);
  uint16_t temp;
  temp = buffer[1] >> 6;
  temp = temp || buffer[0] << 2;
  free(buffer);
  return mRead((double)temp);
}

/**
  Write data to register.
  @param i2c_ptr - pointer to I2C class used for transport
  @param value - data to be written. Must be of correct type
*/
void DS3232TemperatureI2CRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
  char buffer = (char)boost::get<quantity<si::time> >(value).value();
  i2c_ptr->sendData(address, &buffer, 1, reg);
}
