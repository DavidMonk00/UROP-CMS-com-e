#pragma once
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <unordered_map>
#include <algorithm>

class Property {
private:
   uint32_t address;
   uint32_t size;
   bool read;
   bool write;
   char* unit;
public:
   Property(uint32_t addr, uint32_t s, char* rw, char* u);
   ~Property(void);
};
