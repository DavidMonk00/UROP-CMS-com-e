/**
  devices.h
  Purpose: defines Property and device class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "I2C.h"
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
  uint32_t address;
  I2C_base* i2c;
public:
  unordered_map<string, Property*> properties;
  Device(uint32_t address, unordered_map<string, Property*> p);
  Device(string I2Ctype, uint32_t address, unordered_map<string, Property*> p);
  ~Device(void);
  void setI2C(string I2Ctype);
  string read(string property);
  vector<string> getKeys(void);
};
