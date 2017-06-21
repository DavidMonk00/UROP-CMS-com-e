#pragma once
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include "I2CSema.h"
using namespace std;

class Property {
private:
   uint32_t address;
   uint32_t size;
   bool read;
   bool write;
   string format;
   string unit;
public:
   Property(uint32_t addr, uint32_t s, string rw, string fmt, string u);
   ~Property(void);
   uint32_t getAddress(void);
   uint32_t getSize(void);
   bool getReadStatus(void);
   bool getWriteStatus(void);
   string getFormat(void);
   string getUnit(void);
};

class Device {
private:
  I2CSema* i2c;
public:
  unordered_map<string, Property*> properties;
  Device(uint32_t address, unordered_map<string, Property*> p);
  ~Device(void);
  string read(string property);
  vector<string> getKeys(void);
};
