#include <string>
#include <cstdlib>
#include <unordered_map>
#include <stdlib.h>
#include <functional>
#include <stdio.h>
#include "I2CRegister.h"
#include "I2C_base.h"

class I2CDevice {
private:
  std::unordered_map<std::string, I2CBaseRegister*> registers;
  I2CBaseRegister* i2c_reg;
  I2C_base* i2c;
public:
  I2CDevice(I2C_base* i2c_if);
  ~I2CDevice(void);
  uint32_t read(std::string reg);
  void write(std::string reg, uint32_t value);
};
