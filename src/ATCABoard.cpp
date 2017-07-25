/**
  @file ATCABoard.cpp
  @brief Defines functions for the ATCABoard class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "ATCABoard.h"

/**
   @brief Class constructor.
   Full bus/device/register map is defined here. Also requests downstream bus from arbiter.
   @param i2c_type - Pointer to I2C_base transport object.
*/
ATCABoard::ATCABoard(I2C_base* i2c_type) {
   i2c = i2c_type;
   i2c_set = true;
   downstream_available = false;
   requestBus();
   bus_map.insert({
      "1", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"PCI-Clock", new I2CDevice(PCICLOCK_ADDR, std::unordered_map<std::string, I2CBaseRegister*>{
            {"vendorID", new GenericI2CRegister(0x06|0x80, "r", [](int value){return value;},
                                                                 [](units_variant value) {return boost::get<int>(value);})},
            {"deviceID", new GenericI2CRegister(0x07|0x80, "r", [](int value){return value;},
                                                                 [](units_variant value) {return boost::get<int>(value);})},
            {"clock-frequency", new GenericI2CRegister(0x00|0x80, "r", [](int value){quantity<frequency> val = value & 1 ? 100e6*hertz : 133e6*hertz; return val;},
                                                                       [](units_variant value) {return boost::get<int>(value);})},
            {"PLLmode", new PCIClockPLLModeRegister(0x00|0x80, "rw")},
            {"outputenable", new PCIClockOutputEnableRegister(0x01|0x80, "rw")},
         })}
      })
   });
   bus_map.insert({
      "internal", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"CPU", new I2CDevice(0x00, std::unordered_map<std::string, I2CBaseRegister*>{
            {"temperature", new InternalRegister(EAPI_ID_HWMON_CPU_TEMP, "r", [](int value){double temp = ((double)value)/10; return temp*kelvin;})}
         })}
      })
   });
}

/**
   @brief Class constructor.
   Full bus/device/register map is defined here. Also requests downstream bus from arbiter.
   @param i2c_type - Pointer to I2C_base transport object.
*/
ATCABoard::ATCABoard(std::string i2c_string) {
   if (i2c_string == "SEMA") {
      i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
      i2c_set = true;
      downstream_available = false;
      requestBus();
   } else {
      exit(-1);
   }
   bus_map.insert({
      "1", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"PCI-Clock", new I2CDevice(PCICLOCK_ADDR, std::unordered_map<std::string, I2CBaseRegister*>{
            {"vendorID", new GenericI2CRegister(0x06|0x80, "r", [](int value){return value;},
                                                                 [](units_variant value) {return boost::get<int>(value);})},
            {"deviceID", new GenericI2CRegister(0x07|0x80, "r", [](int value){return value;},
                                                                 [](units_variant value) {return boost::get<int>(value);})},
            {"clock-frequency", new GenericI2CRegister(0x00|0x80, "r", [](int value){quantity<frequency> val = value & 1 ? 100e6*hertz : 133e6*hertz; return val;},
                                                                       [](units_variant value) {return boost::get<int>(value);})},
            {"PLLmode", new PCIClockPLLModeRegister(0x00|0x80, "rw")},
            {"outputenable", new PCIClockOutputEnableRegister(0x01|0x80, "rw")},
         })}
      })
   });
   bus_map.insert({
      "internal", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"CPU", new I2CDevice(0x00, std::unordered_map<std::string, I2CBaseRegister*>{
            {"temperature", new InternalRegister(EAPI_ID_HWMON_CPU_TEMP, "r", [](int value){double temp = ((double)value)/10; return temp*kelvin;})},
            {"voltage", new InternalRegister(EAPI_ID_HWMON_VOLTAGE_VCORE, "r", [](int value){double temp = ((double)value)/1000; return temp*volts;})}
         })},
         {"System", new I2CDevice(0x00, std::unordered_map<std::string, I2CBaseRegister*>{
            {"temperature", new InternalRegister(EAPI_ID_HWMON_SYSTEM_TEMP, "r", [](int value){double temp = ((double)value)/10; return temp*kelvin;})},
            {"running-time", new InternalRegister(EAPI_ID_BOARD_RUNNING_TIME_METER_VAL, "r", [](int value){double temp = ((double)value)*60; return temp*seconds;})},
            {"voltage33", new InternalRegister(EAPI_ID_HWMON_VOLTAGE_3V3, "r", [](int value){double temp = ((double)value)/1000; return temp*volts;})},
            {"boot-counter", new InternalRegister(EAPI_ID_BOARD_BOOT_COUNTER_VAL, "r", [](int value){return value;})}
         })}
      })
   });
}

/**
   @brief Class destructor.
   Gives up downstream bus to arbiter.
*/
ATCABoard::~ATCABoard(void) {
   if (downstream_available) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
   delete i2c;
}

/**
   @brief Check if downstream bus is available to access.
   @return Boolean value, true if available.
*/
bool ATCABoard::checkAvailability(void) {
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   return (buffer >> 1) & 0b1;
}

/**
   @brief Requests downstream bus from arbiter. Throws error if not available.
*/
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
      printf("Bus not available.\n"); //TODO throw exception instead
      downstream_available = false;
      exit(-1);
   }
}

/**
   @brief Set which bus to use within I2C fan out.
   @param - 8 bit integer. Each bit corresponds to output bus. Set bit to one to open bus.
*/
void ATCABoard::setFanOut(uint8_t buses) {
   buffer = buses;
   i2c->sendData(ATCA_FANOUT, (char*)&buffer, 1, 0x00);
}

/**
  @brief Set bus to communicate over.
  @param bus - String identifier for bus.
*/
void ATCABoard::setBus(std::string bus) {
   i2c_bus = bus_map[bus];
   uint8_t b = 0b1 << atoi(bus.c_str());
   setFanOut(b);
}

/**
  @brief Set device to communicate to.
  @param device - String identifier for device.
*/
void ATCABoard::setDevice(std::string device) {
   i2c_bus->setDevice(device);
   if (i2c_set) {
      i2c_bus->setI2CType(i2c);
   }
}

/**
  @brief Set device to communicate to.
  @param bus - String identifier for bus.
  @param device - String identifier for device.
*/
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
