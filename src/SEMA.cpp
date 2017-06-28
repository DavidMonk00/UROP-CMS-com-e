/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk
  @version 1.0
*/

//#include "Measurement.h"
//#include "parser.h"
#include "I2CDevice.h"
#include "I2C.h"
#include "I2C_define.h"
#include <string>
#include <stdio.h>
using namespace std;

int main() {
  /*Measurement* M = new Measurement("DS3232");
  M->printProperties();
  string s = M->read("seconds");
  delete M;
  printf("%s\n", s.c_str());
  Parser P;
  phys_quant value = P.getQuantity("9.81 kg m s^-2");*/
  I2CDevice d = I2CDevice(new I2CSema(EAPI_ID_I2C_EXTERNAL, DS3232_ADDR));
  uint8_t x = d.read("seconds");
  printf("%X\n", x);
  d.write("SRAM0", 'H');
  x = d.read("SRAM0");
  printf("%d\n", x);
  return 0;
}
