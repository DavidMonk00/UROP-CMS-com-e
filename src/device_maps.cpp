#include "device_maps.h"

namespace registers {
   register_map DS3232 = {{"seconds",     new TimeI2CRegister(0x00, [](double t){return t*si::seconds;},
                                                                        [](units_variant value) {return (uint8_t)boost::get<quantity<si::time> >(value).value();})},
                              {"temperature", new DS3232TemperatureI2CRegister(0x11, [](double value){return value*kelvin;},
                                                                           [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})},
                              {"SRAM0",       new GenericI2CRegister(0x14, [](double value){return value;},
                                                                           [](units_variant value) {return (uint8_t)boost::get<double>(value);})},
                              {"SRAM1",       new GenericI2CRegister(0x14, [](double value){return value*kelvin;},
                                                                           [](units_variant value) {return (uint8_t)boost::get<quantity<temperature> >(value).value();})}};
} /* registers */
