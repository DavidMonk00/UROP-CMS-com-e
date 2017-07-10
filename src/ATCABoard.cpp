#include "ATCABoard.h"

ATCABoard::ATCABoard(I2C_base* i2c_type) {
   i2c = i2c_type;
   downstream_available = false;
   requestBus();
   bus_map.insert({
      "0", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"PCI Clock", new I2CDevice(PCICLOCK_ADDR, std::unordered_map<std::string, I2CBaseRegister*>{
            {"Vendor ID",   new GenericI2CRegister(0x06|0x80, "rw",     [](uint8_t value){return value;},
                                                                        [](units_variant value) {return boost::get<uint8_t>(value);})},
            {"Device ID",   new GenericI2CRegister(0x07|0x80, "rw",     [](uint8_t value){return value;},
                                                                        [](units_variant value) {return boost::get<uint8_t>(value);})}
         })}
      })
   });
}

ATCABoard::~ATCABoard(void) {
   if (downstream_available) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
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
   i2c->sendData(ATCA_U21, (char*)&buffer, 1, 0x00);
}

void ATCABoard::setBus(std::string bus) {
   i2c_bus = bus_map[bus];
   setFanOut(atoi(bus.c_str()));
}

/**
   @brief Get the map.
   @return Unordered map of string identifiers and I2CBus pointers.
*/
std::unordered_map<std::string, I2CBus*> ATCABoard::getMap(void) {
   return bus_map;
}
