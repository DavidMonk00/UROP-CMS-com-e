/**
  Measurement.h
  Purpose: defines Measurment class.
  @author David Monk
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
#include "devices.h"
#include "I2C.h"
using namespace std;

/**
   Wrapper around device class. Takes string based input for register ID.
*/
class Measurement {
private:
  Device* device;
public:
  Measurement(string d);
  ~Measurement(void);
  string read(string property);
  void printProperties(void);
};
