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
   bus_map.insert({
      "internal", new I2CBus(std::unordered_map<std::string, I2CDevice*> {
         {"CPU", new I2CDevice(0x00, std::unordered_map<std::string, I2CBaseRegister*>{
            {"temperature", new InternalRegister(EAPI_ID_HWMON_CPU_TEMP, "r", [](int value){double temp = ((double)value)/10; return temp*kelvin;})},
            {"voltage", new InternalRegister(EAPI_ID_HWMON_VOLTAGE_VCORE, "r", [](int value){double temp = ((double)value)/1000; return temp*volts;})}
         })},
         {"System", new I2CDevice(0x00, std::unordered_map<std::string, I2CBaseRegister*>{
            {"temperature", new InternalRegister(EAPI_ID_HWMON_SYSTEM_TEMP, "r", [](int value){double temp = ((double)value)/10; return temp*kelvin;})},
            {"running-time", new InternalRegister(EAPI_ID_BOARD_RUNNING_TIME_METER_VAL, "r", [](int value){double temp = ((double)value)*60; return temp*seconds;})},
            {"voltage25", new InternalRegister(EAPI_ID_HWMON_VOLTAGE_2V5, "r", [](int value){double temp = ((double)value)/1000; return temp*volts;})},
            {"voltage33", new InternalRegister(EAPI_ID_HWMON_VOLTAGE_3V3, "r", [](int value){double temp = ((double)value)/1000; return temp*volts;})},
            {"voltage5", new InternalRegister(EAPI_ID_HWMON_VOLTAGE_5V, "r", [](int value){double temp = ((double)value)/1000; return temp*volts;})},
            {"boot-counter", new InternalRegister(EAPI_ID_BOARD_BOOT_COUNTER_VAL, "r", [](int value){return value;})}
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
