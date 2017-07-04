#pragma once

#include <unordered_map>
#include <string>
#include "I2CBus.h"
#include "I2C_define.h"

class Board {
protected:
   std::unordered_map<std::string, I2CBus*> bus_map;
   I2CBus* i2c_bus;
public:
   virtual std::unordered_map<std::string, I2CBus*> getMap(void) = 0;
   void setBus(std::string bus);
   void setDevice(std::string device);
   void setDevice(std::string bus, std::string device);
   void setI2CType(std::string I2CType);
};
