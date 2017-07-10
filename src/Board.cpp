/**
  @file Board.cpp
  @brief Defines functions for the Board base class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "Board.h"

/**
  @brief Set bus to communicate over.
  @param bus - String identifier for bus.
*/
void Board::setBus(std::string bus) {
   i2c_bus = bus_map[bus];
}

/**
  @brief Set device to communicate to.
  @param device - String identifier for device.
*/
void Board::setDevice(std::string device) {
   i2c_bus->setDevice(device);
}

/**
  @brief Set device to communicate to.
  @param bus - String identifier for bus.
  @param device - String identifier for device.
*/
void Board::setDevice(std::string bus, std::string device) {
   setBus(bus);
   i2c_bus->setDevice(device);
   ToString T;
   boost::apply_visitor( T , value );
   return T.mRet;
}

/**
  @brief Set I2C protocol.
  @param i2c_type - Pointer to I2C_base object to be used for I2C communication.
*/
void Board::setI2CType(I2C_base* i2c_type) {
   i2c_bus->setI2CType(i2c_type);
}

/**
   @brief Get available devices on selected bus.
   @return Vector of string IDs for available devices.
*/
std::vector<std::string> Board::getDevices(void) {
   return i2c_bus->getDevices();
}

/**
   @brief Get available devices on bus.
   @param bus - String identifier for bus.
   @return Vector of string IDs for available devices.
*/
std::vector<std::string> Board::getDevices(std::string bus) {
   I2CBus* i2c_bus_temp = bus_map[bus];
   return i2c_bus_temp->getDevices();
}

/**
  @brief Get available properties for selected device.
  @return Vector of strings for each available property.
*/
std::vector<std::string> Board::getProperties(void) {
   return i2c_bus->getProperties();
}

/**
  @brief Get available properties for device.
  @param device - String identifier for device.
  @return Vector of strings for each available property.
*/
std::vector<std::string> Board::getProperties(std::string device) {
   return i2c_bus->getProperties(device);
}

/**
  @brief Get available properties for selected device.
  @param bus - String identifier for bus.
  @param device - String identifier for device.
  @return Vector of strings for each available property.
*/
std::vector<std::string> Board::getProperties(std::string bus, std::string device) {
   I2CBus* i2c_bus_temp = bus_map[bus];
   return i2c_bus_temp->getProperties(device);
}

/**
   @brief Read value of property.
   @param property - String identifier for property.
   @return String of value and assosciated unit.
*/
std::string Board::read(std::string property) {
   units_variant value = i2c_bus->read(property);
   return boost::units::to_string(value);
}

/**
   @brief Write value to property.
   @param property - String identifier for property.
   @param value - String of value and assosciated unit.
*/
void Board::write(std::string property, std::string value) {
   Parser P;
   units_variant var = P.getQuantity(value);
   i2c_bus->write(property, var);
}
