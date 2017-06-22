/**
  I2C_base.cpp
  Purpose: defines functions for the base I2C class.
  @author David Monk
  @version 1.0
*/

#include "I2C_base.h"

/**
  Class constructor.
*/
I2C_base::I2C_base(void) {}

/**
  Class destructor.
*/
I2C_base::~I2C_base(void) {}

/**
  Generic template for sendData function.
  @param buffer - pointer to array of data to be sent
*/
void I2C_base::sendData(char* data) {}

/**
  Generic template for receiveData function.
  @param buffer - pointer to empty array for data
  @return pointer to array of data
*/
char* I2C_base::receiveData(char* buffer) {
  return buffer;
}
