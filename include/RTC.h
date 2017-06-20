/**
  RTC.h
  Purpose: defines RTC class.
  @author David Monk
  @version 1.0
*/

#pragma once
#include <cstdlib>
#include <stdio.h>
#include "I2CSema.h"
#include "I2C_define.h"
using namespace std;

class RTC {
private:
  I2CSema* i2c;
public:
  RTC(void);
  ~RTC(void);
};
