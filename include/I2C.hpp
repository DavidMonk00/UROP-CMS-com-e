/**
  I2C.h
  Purpose: defines derived I2C classes.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include <string>
#include <Sema.h>
#include "diagnostics.hpp"
#include "I2C_base.hpp"
#include "I2C_define.hpp"
using namespace std;

/**
  Derived class from I2C which implements SEMA API function calls to communicate
  with bus.
*/
class I2CSema: public I2C_base {
private:
  uint32_t handler;
  uint32_t id;
  Diagnostics* diagnostics;
public:
  I2CSema(uint32_t ID, uint32_t address, bool diag);
  I2CSema(uint32_t ID);
  ~I2CSema(void);
  uint32_t getBusCap(void);
  void receiveData(uint32_t address, char* buffer, uint32_t bytecnt, uint32_t start_point);
  void sendData(uint32_t address, char* buffer, uint32_t bytecnt, uint32_t start_point);
  void getBoardValue(uint32_t value, uint32_t* buffer);
};
