/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "I2C.h"
#include "COMETestBoard.h"
using namespace std;

int main(int argc, char* argv[]) {
   std::string a;
   a = argc > 1 ? argv[1] : "200K";
   Board* board = new COMETestBoard();
   board->setDevice("0", "DS3232");
   for (auto i : board->getDevices()) { std::cout << i << '\n'; }
   for (auto i : board->getProperties("0","DS3232")) { std::cout << i << '\n'; }
   board->setI2CType(new I2CSema(EAPI_ID_I2C_EXTERNAL));
   for (int i = 0; i < 10; i++) {
      cout << board->read("seconds") << endl;
      //sleep(1);
   }
   board->write("SRAM1", a);
   cout << "SRAM1: " << board->read("SRAM1") << endl;
   return 0;
}
