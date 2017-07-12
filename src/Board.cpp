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

std::vector<std::string> Board::getProperties(void) {
   return i2c_bus->getProperties();
}

std::string Board::read(std::string property) {
   units_variant value = i2c_bus->read(property);
   ToString T;
   boost::apply_visitor( T , value );
   return T.mRet;
}

void Board::write(std::string property, std::string value) {
   Parser P;
   units_variant var = P.getQuantity(value);
   i2c_bus->write(property, var);
}
