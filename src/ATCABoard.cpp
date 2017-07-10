#include "ATCABoard.h"

ATCABoard::ATCABoard(I2C_base* i2c_type) {
   i2c = i2c_type;
   i2c_set = true;
   downstream_available = false;
   requestBus();
   bus_map.insert({
      "1", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"PCI Clock", new I2CDevice(PCICLOCK_ADDR, std::unordered_map<std::string, I2CBaseRegister*>{
            {"vendor ID", new GenericI2CRegister(0x06|0x80, "r", [](int value){return value;},
                                                                 [](units_variant value) {return boost::get<int>(value);})},
            {"device ID", new GenericI2CRegister(0x07|0x80, "r", [](int value){return value;},
                                                                 [](units_variant value) {return boost::get<int>(value);})},
            {"clock frequency", new GenericI2CRegister(0x00|0x80, "r", [](int value){quantity<frequency> val = value & 1 ? 100e6*hertz : 133e6*hertz; return val;},
                                                                       [](units_variant value) {return boost::get<int>(value);})},
         })}
      })
   });
}

ATCABoard::~ATCABoard(void) {
   if (downstream_available) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
   delete i2c;
}

bool ATCABoard::checkAvailability(void) {
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   return (buffer >> 1) & 0b1;
}

void ATCABoard::requestBus(void) {
   buffer = 0x7b; //enable interrupt
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x05);
   buffer = 0x00; //set request time
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x03);
   buffer = 0x01; //request downstream
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   if (checkAvailability()) {
      buffer = 0x05; //enable downstream
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
      downstream_available = true;
   } else {
      std::cout << "Bus not available." << '\n';
      downstream_available = false;
      exit(-1);
   }
}

void ATCABoard::setFanOut(uint8_t buses) {
   buffer = buses;
   i2c->sendData(ATCA_FANOUT, (char*)&buffer, 1, 0x00);
}

void ATCABoard::setBus(std::string bus) {
   i2c_bus = bus_map[bus];
   uint8_t b = 0b1 << atoi(bus.c_str());
   setFanOut(b);
}

void ATCABoard::setDevice(std::string device) {
   i2c_bus->setDevice(device);
   if (i2c_set) {
      i2c_bus->setI2CType(i2c);
   }
}

void ATCABoard::setDevice(std::string bus, std::string device) {
   setBus(bus);
   i2c_bus->setDevice(device);
   if (i2c_set) {
      i2c_bus->setI2CType(i2c);
   }
}

/**
   @brief Get the map.
   @return Unordered map of string identifiers and I2CBus pointers.
*/
std::unordered_map<std::string, I2CBus*> ATCABoard::getMap(void) {
   return bus_map;
}
