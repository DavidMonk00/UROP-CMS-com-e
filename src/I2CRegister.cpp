/**
  @file I2CRegister.cpp
  @brief Defines functions for the derived I2CBaseRegister classes.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CRegister.h"

/**
  @brief Class constructor.
  @param r - Address of register within I2C device.
  @param read_func - Lambda function to convert hexadecimal value into correct units type.
  @param write_func - Lambda function to convert units type into uint8_t to be written.
*/
GenericI2CRegister::GenericI2CRegister(uint32_t r, std::string rw, std::function<units_variant(double)> read_func,
                                                      std::function<uint8_t(units_variant)> write_func) {
  reg = r;
  mRead = read_func;
  mWrite = write_func;
  const char* rw_char = rw.c_str();
  const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
  const char* position = std::find(rw_char, end, 'r');
  b_read = position != end;
  position = std::find(rw_char, end, 'w');
  b_write = position != end;
}

/**
   @brief Class destructor.
*/
GenericI2CRegister::~GenericI2CRegister(void) {}

/**
  @brief Read data from register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @return units_variant containing quantity of correct type.
*/
units_variant GenericI2CRegister::read(I2C_base* i2c_ptr, uint32_t address) {
   if (b_read) {
      char buffer = 0;
      i2c_ptr->receiveData(address, &buffer, 1, reg);
      return mRead(buffer);
   } else {
      printf("Register cannot be read.\n");
      exit(-1);
   }
}

/**
  @brief Write data to register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @param value - Data to be written. Must be of correct type.
*/
void GenericI2CRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
   if (b_write) {
      char buffer = mWrite(value);
      i2c_ptr->sendData(address, &buffer, 1, reg);
   } else {
      printf("Register cannot be written to.\n");
      exit(-1);
   }
}

/**
  @brief Class constructor.
  @param r - Address of register within I2C device.
  @param read_func - Lambda function to convert hexadecimal value into correct units type
  @param write_func - Lambda function to convert units type into uint8_t to be written.
*/
TimeI2CRegister::TimeI2CRegister(uint32_t r, std::string rw, std::function<units_variant(double)> read_func,
                                                std::function<uint8_t(units_variant)> write_func) {
  reg = r;
  mRead = read_func;
  mWrite = write_func;
  const char* rw_char = rw.c_str();
  const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
  const char* position = std::find(rw_char, end, 'r');
  b_read = position != end;
  position = std::find(rw_char, end, 'w');
  b_write = position != end;
}

/**
   @brief Class destructor.
*/
TimeI2CRegister::~TimeI2CRegister(void) {}

/**
  @brief Read data from register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @return units_variant containing quantity of correct type.
*/
units_variant TimeI2CRegister::read(I2C_base* i2c_ptr, uint32_t address) {
   if (b_read) {
      char buffer = 0;
      i2c_ptr->receiveData(address, &buffer, 1, reg);
      double t = 10*(buffer/16) + buffer%16;
      return mRead(t);
   } else {
      printf("Register cannot be read.\n");
      exit(-1);
   }
}

/**
  @brief Write data to register.
  @param i2c_ptr - pointer to I2C_base class used for transport
  @param value - Data to be written. Must be of correct type.
*/
void TimeI2CRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
   if (b_write) {
      char buffer = (char)mWrite(value);
      i2c_ptr->sendData(address, &buffer, 1, reg);
   } else {
      printf("Register cannot be written to.\n");
      exit(-1);
   }
}

/**
  @brief Class constructor.
  @param r - Address of register within I2C device.
  @param read_func - Lambda function to convert hexadecimal value into correct units type.
  @param write_func - Lambda function to convert units type into uint8_t to be written.
*/
DS3232TemperatureI2CRegister::DS3232TemperatureI2CRegister(uint32_t r, std::string rw, std::function<units_variant(double)> read_func,
                                                                          std::function<uint8_t(units_variant)> write_func) {
  reg = r;
  mRead = read_func;
  mWrite = write_func;
  const char* rw_char = rw.c_str();
  const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
  const char* position = std::find(rw_char, end, 'r');
  b_read = position != end;
  position = std::find(rw_char, end, 'w');
  b_write = position != end;
}

/**
   @brief Class destructor.
*/
DS3232TemperatureI2CRegister::~DS3232TemperatureI2CRegister(void) {}

/**
  @brief Read data from register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @return units_variant containing quantity of correct type.
*/
units_variant DS3232TemperatureI2CRegister::read(I2C_base* i2c_ptr, uint32_t address) {
   if (b_read) {
      char* buffer = (char*)malloc(2*sizeof(char));
      i2c_ptr->receiveData(address, buffer, 2, reg);
      uint16_t temp;
      temp = buffer[1] >> 6;
      temp = temp || buffer[0] << 2;
      free(buffer);
      return mRead((double)temp);
   } else {
      printf("Register cannot be read.\n");
      exit(-1);
   }
}

/**
  @brief Write data to register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @param value - Data to be written. Must be of correct type.
*/
void DS3232TemperatureI2CRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
   if (b_write) {
      char buffer = (char)boost::get<quantity<si::time> >(value).value();
      i2c_ptr->sendData(address, &buffer, 1, reg);
   } else {
      printf("Register cannot be written to.\n");
      exit(-1);
   }
}
