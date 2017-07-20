#pragma once
#include <string>
#include "I2C.h"

class I2CRaw {
private:
   uint8_t buffer;
   I2C_base* i2c;
   bool checkAvailability(void);
   void requestBus(void);
   void setFanOut(uint8_t buses);
public:
   I2CRaw(void);
   ~I2CRaw();
   int read(uint32_t address, uint32_t start_point);
   void write(uint32_t address, uint8_t buffer, uint32_t start_point);
};
