#pragma once
#include <cstdlib>
#include <stdio.h>
#include <Sema.h>
#include "diagnostics.h"
#include "I2C.h"
using namespace std;

class I2CSema: public I2C {
private:
  uint32_t handler;
  uint32_t id;
public:
  I2CSema(void);
  ~I2CSema(void);
  uint32_t getBusCap(void);
  char* receiveData(char* buffer, uint32_t ByteCnt); //Receive data
  void sendData(char* buffer, uint32_t ByteCnt); //Send data
};
