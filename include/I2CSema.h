#pragma once
#include <cstdlib>
#include <stdio.h>
#include <Sema.h>
#include "diagnostics.h"
#include "I2C.h"
#include "I2C_define.h"
using namespace std;

/**
  Derived class from I2C which implements SEMA function calls to communicate
  with bus.
*/
class I2CSema: public I2C {
private:
  uint32_t handler;
  uint32_t id;
public:
  I2CSema(uint32_t ID, uint8_t address, bool diag);
  I2CSema(uint32_t ID, uint8_t address);
  ~I2CSema(void);
  uint32_t getBusCap(void);
  char* receiveData(char* buffer, uint32_t bytecnt, uint32_t start_point);
  void sendData(char* buffer, uint32_t bytecnt, uint32_t start_point);
};
