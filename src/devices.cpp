#include "devices.h"

Property::Property(uint32_t addr, uint32_t s, char* rw, char* u) {
   address = addr;
   size = s;
   char* end = rw + sizeof(rw)/sizeof(rw[0]);
   char* position = std::find(rw, end, 'r');
   read = position != end;
   position = std::find(rw, end, 'w');
   write = position != end;
   unit = u;
}

Property::~Property(void) {
   
}
