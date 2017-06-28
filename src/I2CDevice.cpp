#include "I2CDevice.h"

auto lambda_read = [](I2C_base* i2c_ptr){char buffer = 0;i2c_ptr->receiveData(&buffer, 1, 0x00);return buffer*seconds;};


//register_map r = {{"temperature", new GenericI2CRegister(0x01, lambda_read,lambda_write)},
//                                                       {"humidity",    new GenericI2CRegister(0x02, lambda_read,lambda_write)}};

register_map DS3232_reg = {{"seconds",     new TimeI2CRegister(0x00, [](double t){return t*si::seconds;})},
                           {"temperature", new GenericI2CRegister(0x12, [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})},
                           {"SRAM0",       new GenericI2CRegister(0x14, [](double value){return value;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<double>(value);})},
                           {"SRAM1",       new GenericI2CRegister(0x14, [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})}};

I2CDevice::I2CDevice(I2C_base* i2c_if) {
  i2c = i2c_if;
  registers = DS3232_reg;
}

I2CDevice::~I2CDevice(void) {}

units_variant I2CDevice::read(std::string reg) {
  i2c_reg = registers[reg];
  I2C_base* ptr;
  return i2c_reg->read(i2c);
}

void I2CDevice::write(std::string reg, units_variant value) {
  i2c_reg = registers[reg];
  I2C_base* ptr;
  i2c_reg->write(i2c, value);
}
