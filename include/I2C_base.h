/**
  I2C_base.h
  Purpose: defines I2C base class.
  @author David Monk
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
  I2C_base(void); //Initialise class
  ~I2C_base(void); //Class destructor
  void sendData(char* data); //Send data
  char* receiveData(char* buffer); //Receive data
};
