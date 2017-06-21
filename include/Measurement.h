#pragma once
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include "devices.h"
using namespace std;

class Measurement {
private:
  Device* device;
public:
  Measurement(string d);
  ~Measurement(void);
  string read(string property);
  void printProperties(void);
};
