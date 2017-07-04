#include "I2CBus.h"

//std::unordered_map<std::string, I2CDevice*>

I2CBus::I2CBus(std::unordered_map<std::string, I2CDevice*> device_map) {
   devices = device_map;
}

I2CBus::~I2CBus(void) {}

void I2CBus::setDevice(std::string device_id) {
   device = devices[device_id];
}

std::vector<std::string> I2CBus::getProperties(void) {
   return device->getProperties();
}

units_variant I2CBus::read(std::string property) {
   return device->read(property);
}

void I2CBus::write(std::string property, units_variant value) {
   device->write(property, value);
}
