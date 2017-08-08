/**
  @file I2CBus.cpp
  @brief Defines functions for the I2CBus class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CBus.hpp"

/**
   @brief Class constructor.
   @param device_map - Unordered map of string identifiers and I2CDevice pointers.
*/
I2CBus::I2CBus(std::unordered_map<std::string, I2CDevice*> device_map) {
   devices = device_map;
}

/**
   @brief Class destructor.
*/
I2CBus::~I2CBus(void) {}

/**
   @brief Set device.
   @param device_id - String identifier of device.
*/
void I2CBus::setDevice(std::string device_id) {
   device = devices[device_id];
}

/**
   @brief Set I2C protocol to be used for communication.
   @param i2c_type - I2C_base pointer for transport object.
*/
void I2CBus::setI2CType(I2C_base* i2c_type) {
   device->setI2CType(i2c_type);
}

/**
   @brief Get properties of selected device.
   @return Vector of strings of each property for the selected device.
*/
std::vector<std::string> I2CBus::getProperties(void) {
   return device->getProperties();
}

/**
   @brief Get properties of device.
   @param device - String identifier for device.
   @return Vector of strings of each property for the device.
*/
std::vector<std::string> I2CBus::getProperties(std::string device_ID) {
   I2CDevice* device_temp = devices[device_ID];
   return device_temp->getProperties();
}

/**
   @brief Get devices on bus.
   @return Vector of strings of each device ID.
*/
std::vector<std::string>I2CBus::getDevices(void) {
  std::vector<std::string> v;
  v.reserve(devices.size());
  for (auto kv : devices) {
    v.push_back(kv.first);
  }
  return v;
}

/**
   @brief Read property from device.
   @param property - String identifier for register to be read.
   @return units_variant of value and associated unit.
*/
units_variant I2CBus::read(std::string property) {
   return device->read(property);
}

/**
   @brief Write property to device.
   @param property - String identifier for register to be written to.
   @param units_variant of value and associated unit to be written.
*/
void I2CBus::write(std::string property, units_variant value) {
   device->write(property, value);
}
