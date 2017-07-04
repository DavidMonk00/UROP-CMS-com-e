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
#include "parser.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "COMETestBoard.h"
#include "I2CBus.h"
using namespace std;

int main(int argc, char* argv[]) {
   std::string a;
   a = argc > 1 ? argv[1] : "3.14159";
   Parser P;
   units_variant var = P.getQuantity(a);
   std::cout << var << '\n';
   Board* board = new COMETestBoard();
   //board->setBus("0");
   board->setDevice("0", "DS3232");
   I2CBus* bus = board->getMap()["0"];
   bus->setDevice("DS3232");
   bus->setI2CType("SEMA");
   for (auto i : bus->getProperties()) {
      std::cout << i << '\n';
   }
   units_variant x = bus->read("seconds");
   cout << x << endl;
   x = bus->read("temperature");
   cout << x << endl;
   quantity<temperature> t = 61*kelvin;
   bus->write("SRAM1", t);
   x = bus->read("SRAM1");
   cout << x << endl;
   return 0;
}
