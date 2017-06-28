#include "I2CRegister.h"

GenericI2CRegister::GenericI2CRegister(uint32_t addr, std::function<uint8_t(I2C_base*, uint32_t)> read_func, std::function<void(I2C_base*, uint32_t, uint8_t)> write_func) {
  address = addr;
  mRead = read_func;
  mWrite = write_func;
}

GenericI2CRegister::~GenericI2CRegister(void) {}

uint32_t GenericI2CRegister::read(I2C_base* i2c_ptr) {
   return mRead(i2c_ptr, address);
}

void GenericI2CRegister::write(I2C_base* i2c_ptr, uint8_t value) {
   mWrite(i2c_ptr, address, value);
}
