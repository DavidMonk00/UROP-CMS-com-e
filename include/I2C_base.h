/**
  I2C_base.h
  Purpose: defines I2C base class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include <cstdlib>
#include <stdio.h>
#include <stdint.h>
using namespace std;

/**
  Base I2C class. Has no functionality in current state but acts as a basis for
  derived I2C classes
*/
class I2C_base {
protected:
  uint32_t addr;
public:
  virtual void receiveData(uint32_t address, char* buffer, uint32_t bytecnt, uint32_t start_point)=0;
  virtual void sendData(uint32_t address, char* buffer, uint32_t bytecnt, uint32_t start_point)=0;
  virtual void receiveDataRaw(uint32_t address, char* buffer, uint32_t bytecnt) = 0;
  virtual void sendDataRaw(uint32_t address, char* buffer, uint32_t bytecnt) = 0;
};
