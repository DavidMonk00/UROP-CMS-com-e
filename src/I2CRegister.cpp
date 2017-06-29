#include "I2CRegister.h"

GenericI2CRegister::GenericI2CRegister(uint32_t addr, std::function<units_variant(double)> read_func, std::function<uint8_t(units_variant)> write_func) {
  address = addr;
  mRead = read_func;
  mWrite = write_func;
}

GenericI2CRegister::~GenericI2CRegister(void) {}

units_variant GenericI2CRegister::read(I2C_base* i2c_ptr) {
  char buffer = 0;
  i2c_ptr->receiveData(&buffer, 1, address);
  return mRead((double)buffer);
}

void GenericI2CRegister::write(I2C_base* i2c_ptr, units_variant value) {
  char buffer = mWrite(value);
  i2c_ptr->sendData(&buffer, 1, address);
}

TimeI2CRegister::TimeI2CRegister(uint32_t addr, std::function<units_variant(double)> read_func) {
  address = addr;
  mRead = read_func;
}

TimeI2CRegister::~TimeI2CRegister(void) {}

units_variant TimeI2CRegister::read(I2C_base* i2c_ptr) {
  char buffer = 0;
  i2c_ptr->receiveData(&buffer, 1, address);
  double t = 10*(buffer/16) + buffer%16;
  return mRead(t);
}

void TimeI2CRegister::write(I2C_base* i2c_ptr, units_variant value) {
  char buffer = (char)boost::get<quantity<si::time> >(value).value();
  i2c_ptr->sendData(&buffer, 1, address);
}
