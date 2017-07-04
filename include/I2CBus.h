#pragma once
#include <string>
#include <unordered_map>
#include <stdio.h>
#include "I2CDevice.h"
#include "units_define.h"

class I2CBus {
private:
   std::unordered_map<std::string, I2CDevice*> devices;
   I2CDevice* device;
public:
   I2CBus(std::unordered_map<std::string, I2CDevice*> device_map);
   ~I2CBus(void);
   void setDevice(std::string device_id);
   std::vector<std::string> getProperties(void);
   units_variant read(std::string property);
   void write(std::string property, units_variant value);
};
