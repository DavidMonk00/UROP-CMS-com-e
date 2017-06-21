#pragma once
#include <cstdlib>
#include <stdio.h>
#include <unordered_map>

class Property {
private:
   uint32_t address;
   uint32_t size;
   bool read;
   bool write;
   char* unit;
public:
   Property(uint32_t addr, uint32_t s, char* rw, char* u);
};
