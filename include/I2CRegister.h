#include <string>
#include <cstdlib>
#include <unordered_map>
#include <stdlib.h>
#include <functional>
#include <stdio.h>
#include "I2C_base.h"

class I2CBaseRegister {
public:
   virtual uint32_t read(I2C_base* i2c_ptr) = 0;
   virtual void write(I2C_base* i2c_ptr, uint8_t value) = 0;
};

class GenericI2CRegister : public I2CBaseRegister {
   uint32_t address;
   std::function<uint8_t(I2C_base*, uint32_t)> mRead;
   std::function<void(I2C_base*, uint32_t, uint8_t)> mWrite;
public:
   GenericI2CRegister(uint32_t addr, std::function<uint8_t(I2C_base*, uint32_t)> read_func, std::function<void(I2C_base*, uint32_t, uint8_t)> write_func);
   ~GenericI2CRegister(void);
   uint32_t read(I2C_base* i2c_ptr);
   void write(I2C_base* i2c_ptr, uint8_t value);
};
