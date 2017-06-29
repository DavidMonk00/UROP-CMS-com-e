/**
  RTC.h
  Purpose: defines RTC class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include <cstdlib>
#include <stdio.h>
#include "I2C.h"
#include "I2C_define.h"
using namespace std;

/**
   Specific class for RTC clock I2C chip.
*/
class RTC {
private:
  I2CSema* i2c;
public:
  RTC(void);
  ~RTC(void);
};
