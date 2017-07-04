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
   COMETestBoard* m = new COMETestBoard();
   I2CBus* bus = m->getMap()["0"];
   bus->setDevice("DS3232");
   bus->setI2CType("SEMA");
   for (auto i : bus->getProperties()) {
      std::cout << i << '\n';
   }
   units_variant x = bus->read("seconds");
   cout << x << endl;
   //printf("%X\n", x);
   /*x = d.read("temperature");
   cout << x << endl;
   quantity<temperature> t = 61*kelvin;
   d.write("SRAM1", t);
   x = d.read("SRAM1");
   //printf("%d\n", x);
   cout << x << endl;*/
   return 0;
}
