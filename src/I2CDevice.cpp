#include "I2CDevice.h"

auto lambda_read = [](I2C_base* i2c_ptr, uint32_t addr) {
   char buffer = 0;
   i2c_ptr->receiveData(&buffer, 1, addr);
   return buffer;
};
auto lambda_write = [](I2C_base* i2c_ptr, uint32_t addr, uint8_t value) {
   char buffer = (char)value;
   i2c_ptr->sendData(&buffer, 1, addr);
};

std::unordered_map<std::string, I2CBaseRegister*> r = {{"temperature", new GenericI2CRegister(0x01, lambda_read,lambda_write)},
                                                       {"humidity",    new GenericI2CRegister(0x02, lambda_read,lambda_write)}};

std::unordered_map<std::string, I2CBaseRegister*> DS3232_reg = {{"seconds",     new GenericI2CRegister(0x00,lambda_read,lambda_write)},
                                                                {"minutes",     new GenericI2CRegister(0x01,lambda_read,lambda_write)},
                                                                {"hours",       new GenericI2CRegister(0x02,lambda_read,lambda_write)},
                                                                {"temperature", new GenericI2CRegister(0x11,lambda_read,lambda_write)},
                                                                {"SRAM0",       new GenericI2CRegister(0x14,lambda_read,lambda_write)}};

I2CDevice::I2CDevice(I2C_base* i2c_if) {
  i2c = i2c_if;
  registers = DS3232_reg;
}

I2CDevice::~I2CDevice(void) {}

uint32_t I2CDevice::read(std::string reg) {
  i2c_reg = registers[reg];
  I2C_base* ptr;
  return i2c_reg->read(ptr);
}

void I2CDevice::write(std::string reg, uint32_t value) {
  i2c_reg = registers[reg];
  I2C_base* ptr;
  i2c_reg->write(ptr, value);
}
