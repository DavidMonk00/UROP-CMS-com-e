#include "devices.h"

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

Property::~Property(void) {}

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

Device::Device(uint32_t addr, unordered_map<string, Property*> p) {
  address = addr;
  properties = p;
}

Device::Device(string I2Ctype, uint32_t addr, unordered_map<string, Property*> p) {
  address = addr;
  properties = p;
  if (I2Ctype == "SEMA") {
    index = 0;
    i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL, address);
  } else {
    printf("No valid I2C protocol specified\n");
    exit(-1);
  }
}

Device::~Device(void) {}

void Device::setI2C(string I2Ctype) {
  if (I2Ctype == "SEMA") {
    index = 0;
    i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL, address);
  } else {
    printf("No valid I2C protocol specified\n");
    exit(-1);
  }
}

string Device::read(string property) {
  Property* p = properties[property];
  if (p->getReadStatus()) {
    char* buffer = (char*)malloc(64*sizeof(char));
    buffer = i2c->receiveData(buffer, p->getSize(), p->getAddress());
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

vector<string>Device::getKeys(void) {
  vector<string> v;
  v.reserve(properties.size());
  for (auto kv : properties) {
    v.push_back(kv.first);
  }
  return v;
}
