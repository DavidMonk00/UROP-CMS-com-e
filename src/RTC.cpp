/**
  RTC.cpp
  Purpose: defines functions for the RTC class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "RTC.h"

/**
   Class constructor.
*/
RTC::RTC(void) {
  i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL, DS3232_ADDR, true);
}

/**
   Class destructor.
*/
RTC::~RTC(void) {
  delete i2c;
}
