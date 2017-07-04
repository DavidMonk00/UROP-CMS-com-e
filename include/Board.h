#pragma once

#include <unordered_map>
#include <string>
#include "I2CBus.h"
#include "I2C_define.h"

class Board {
protected:
   std::unordered_map<std::string, I2CBus*> bus_map;
public:
   virtual std::unordered_map<std::string, I2CBus*> getMap(void) = 0;
};
