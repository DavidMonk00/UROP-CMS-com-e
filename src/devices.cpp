/**
  @file I2C.cpp
  @brief Defines functions for the Property and Device classes.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "devices.h"

/**
  @brief Class constructor.
  @param addr - Address of I2C register within device.
  @param s - Size of property, i.e. the number of registers it spans.
  @param rw - The read/write status of the register.
  @param fmt - Format for output.
  @param u - Unit of output.
*/
Property::Property(uint32_t addr, uint32_t s, string rw, string fmt, string u) {
   address = addr;
   size = s;
   const char* rw_char = rw.c_str();
   const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
   const char* position = std::find(rw_char, end, 'r');
   read = position != end;
   position = std::find(rw_char, end, 'w');
   write = position != end;
   format = fmt;
   unit = u;
 }

/**
   @brief Class destructor.
*/
Property::~Property(void) {}

// --- Get functions for each parameter ---
uint32_t Property::getAddress(void){
  return address;
}

uint32_t Property::getSize(void){
  return size;
}

bool Property::getReadStatus(void) {
  return read;
}

bool Property::getWriteStatus(void) {
  return write;
}

string Property::getFormat(void) {
  return format;
}

string Property::getUnit(void) {
  return unit;
}

/**
  @brief Class constructor.
  @param address - Address of I2C device.
  @param p - Map of properties assosciated with I2C device.
*/
Device::Device(uint32_t addr, unordered_map<string, Property*> p) {
  address = addr;
  properties = p;
}

/**
  @brief Class constructor with user defined I2C API type.
  @param I2Ctype - String defining the I2C API to use.
  @param address - Address of I2C device.
  @param p - Map of properties assosciated with I2C device.
*/
Device::Device(string I2Ctype, uint32_t addr, unordered_map<string, Property*> p) {
  address = addr;
  properties = p;
  if (I2Ctype == "SEMA") {
    i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL, address, true);
  } else {
    printf("No valid I2C protocol specified\n");
    exit(-1);
  }
}

/**
   @brief Class destructor.
*/
Device::~Device(void) {
  delete i2c;
}

/**
  @brief Set I2C type manually.
  @param I2Ctype - Specifies the API to use for I2C calls.
*/
void Device::setI2C(string I2Ctype) {
  if (I2Ctype == "SEMA") {
    i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL, address, true);
  } else {
    printf("No valid I2C protocol specified\n");
    exit(-1);
  }
}

/**
  @brief Read data from register.
  @param property - The property to be read.
  @return String with value and unit.
*/
string Device::read(string property) {
  Property* p = properties[property];
  if (p->getReadStatus()) {
    char* buffer = (char*)malloc(64*sizeof(char));
    i2c->receiveData(0,buffer, p->getSize(), p->getAddress());
    char str[64];
    sprintf(str, p->getFormat().c_str(), buffer[0]);
    for (int i = 1; i < p->getSize(); i++) {
      sprintf(str + strlen(str), p->getFormat().c_str(), buffer[i]);
    }
    sprintf(str + strlen(str), " %s", p->getUnit().c_str());
    string s(str);
    return s;
  } else {
    return "Exception: property cannot be read.";
  }
}

/**
  @brief Get all possible registers for given device.
  @return Vector of strings assosciated with the properties.
*/
vector<string>Device::getKeys(void) {
  vector<string> v;
  v.reserve(properties.size());
  for (auto kv : properties) {
    v.push_back(kv.first);
  }
  return v;
}
