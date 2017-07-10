#include "I2CRegister.h"

PCIClockPLLModeRegister::PCIClockPLLModeRegister(uint32_t addr, std::string rw) {
   reg = addr;
   const char* rw_char = rw.c_str();
   const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
   const char* position = std::find(rw_char, end, 'r');
   b_read = position != end;
   position = std::find(rw_char, end, 'w');
   b_write = position != end;
}

PCIClockPLLModeRegister::~PCIClockPLLModeRegister(void) {}
