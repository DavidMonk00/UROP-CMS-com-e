/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

//#include "Measurement.h"
//#include "parser.h"
#include "I2CDevice.h"
#include "I2C.h"
#include "I2C_define.h"
#include <string>
#include <stdio.h>
#include <iostream>
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
  units_variant x = d.read("seconds");
  cout << x << endl;
  //printf("%X\n", x);
  quantity<temperature> t = 61*kelvin;
  d.write("SRAM1", t);
  x = d.read("SRAM1");
  //printf("%d\n", x);
  cout << x << endl;
  return 0;
}
