#pragma once

#include <unordered_map>
#include <string>
#include "I2CBus.h"
#include "I2C_define.h"

class Map {
   std::unordered_map<std::string, I2CBus*> bus_map;
public:
   Map(void);
   std::unordered_map<std::string, I2CBus*> getMap(void);
};
