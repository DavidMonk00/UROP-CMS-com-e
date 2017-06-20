/**
  RTC.cpp
  Purpose: defines functions for the RTC class.
  @author David Monk
  @version 1.0
*/

#include "RTC.h"

RTC::RTC(void) {
  i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL, DS3232);
}

RTC::~RTC(void) {
  delete i2c;
}
