#include "Board.h"

void Board::setBus(std::string bus) {
   i2c_bus = bus_map[bus];
}

void Board::setDevice(std::string device) {
   i2c_bus->setDevice(device);
}

void Board::setDevice(std::string bus, std::string device) {
   i2c_bus = bus_map[bus];
   i2c_bus->setDevice(device);
}

void Board::setI2CType(I2C_base* i2c_type) {
   i2c_bus->setI2CType(i2c_type);
}
