#pragma once
#include <string>
#include <unordered_map>
#include <stdio.h>
#include "I2CDevice.h"

class I2CBus {
private:
   std::unordered_map<std::string, I2CDevice*> devices;
public:
   I2CBus(std::unordered_map<std::string, I2CDevice*> device_map);
   ~I2CBus(void);
   void setDevice(std::string device_id);
};
