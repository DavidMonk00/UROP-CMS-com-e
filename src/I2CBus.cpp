#include "I2CBus.h"

//std::unordered_map<std::string, I2CDevice*>

I2CBus::I2CBus(std::unordered_map<std::string, I2CDevice*> device_map) {
   devices = device_map;
}

I2CBus::~I2CBus(void) {}
