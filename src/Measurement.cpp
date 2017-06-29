#include "Measurement.h"

unordered_map<string, Property*> DS3232 = {{"seconds", new Property(0x00,1,"rw","%X","seconds")},
                                           {"minutes", new Property(0x01,1,"rw","%X","minutes")},
                                           {"hours", new Property(0x02,1,"rw","%X","hours")},
                                           {"temperature", new Property(0x11,2,"r","%X","kelvin")}};
unordered_map<string, Device*> map_devices = {{"DS3232", new Device(DS3232_ADDR, DS3232)}};

/**
   Class constructor.
   @param d - device name
*/
Measurement::Measurement(string d) {
  device = map_devices[d];
  device->setI2C("SEMA");
}

/**
   Class destructor.
*/
Measurement::~Measurement(void) {
  delete device;
}

/**
   Read property.
   @param property - name of property to write
   @return string of value read
*/
string Measurement::read(string property) {
  return device->read(property);
}

/**
   Print properties available on I2C device.
*/
void Measurement::printProperties(void) {
  vector<string> v = device->getKeys();
  printf("Properties available:\n");
  for (string i : v) {
    printf("'%s'\n", i.c_str());
  }
}
