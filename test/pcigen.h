#pragma once
#include <string>
#include <stdio.h>
#include "I2C.h"

class PCIGen {
public:
  enum tPllBandwidth { HIGH = 0xE , MEDIUM = 0xA , LOW = 0x8 };

private:
  I2CSema* I2C;
public:
  PCIGen(uint32_t addr);
  ~PCIGen(void);
  void setPLLMode( tPllBandwidth mode );
  void enableOutput(void);
};
