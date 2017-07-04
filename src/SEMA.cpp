/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include "I2C.h"
#include "COMETestBoard.h"
using namespace std;

int main(int argc, char* argv[]) {
   std::string a;
   a = argc > 1 ? argv[1] : "200K";
   Board* board = new COMETestBoard();
   board->setDevice("0", "DS3232");
   board->setI2CType(new I2CSema(EAPI_ID_I2C_EXTERNAL));
   std::cout << "Available registers:" << '\n';
   for (auto i : board->getProperties()) {
      std::cout << i << '\n';
   }
   std::string x = board->read("seconds");
   cout << "Seconds: " << x << endl;
   board->write("SRAM1", a);
   x = board->read("SRAM1");
   cout << "SRAM1: " << x << endl;
   return 0;
}
