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
   void setI2CType(I2C_base* i2c_type);
   std::vector<std::string> getProperties(void);
   std::vector<std::string> getProperties(std::string device_ID);
   std::vector<std::string> getDevices(void);
   units_variant read(std::string property);
   void write(std::string property, units_variant value);
};
