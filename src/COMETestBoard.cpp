/**
  @file COMETestBoard.cpp
  @brief Defines functions for the COMETestBoard derived class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "COMETestBoard.h"

/**
   @brief Class constructor.
   Full bus/device/register map is defined here.
*/
COMETestBoard::COMETestBoard(void) {
   bus_map.insert({
      "0", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"DS3232", new I2CDevice(DS3232_ADDR, std::unordered_map<std::string, I2CBaseRegister*>{
            {"seconds",     new TimeI2CRegister(0x00, "r",              [](double t){return t*si::seconds;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<si::time> >(value).value();})},
            {"temperature", new DS3232TemperatureI2CRegister(0x11, "r", [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})},
            {"SRAM0",       new GenericI2CRegister(0x14, "rw",          [](double value){return value;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<double>(value);})},
            {"SRAM1",       new GenericI2CRegister(0x14, "rw",          [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})}
         })}
      })
   });
}

/**
   @brief Class constructor.
   Full bus/device/register map is defined here.
   @param i2c_type - Pointer to I2C transport class to be used.
*/
COMETestBoard::COMETestBoard(I2C_base* i2c_type) {
   i2c = i2c_type;
   bus_map.insert({
      "0", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"DS3232", new I2CDevice(DS3232_ADDR, std::unordered_map<std::string, I2CBaseRegister*>{
            {"seconds",     new TimeI2CRegister(0x00, "r",              [](double t){return t*si::seconds;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<si::time> >(value).value();})},
            {"temperature", new DS3232TemperatureI2CRegister(0x11, "r", [](double value){return value*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})},
            {"SRAM0",       new GenericI2CRegister(0x14, "rw",          [](uint8_t value){return value;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<double>(value);})},
            {"SRAM1",       new GenericI2CRegister(0x14, "rw",          [](uint8_t value){double val = value; return val*kelvin;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})}
         })}
      })
   });
}

/**
   @brief Get the map.
   @return Unordered map of string identifiers and I2CBus pointers.
*/
std::unordered_map<std::string, I2CBus*> COMETestBoard::getMap(void) {
   return bus_map;
}
