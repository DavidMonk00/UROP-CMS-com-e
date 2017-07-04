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
   Board* board = new COMETestBoard();
   board->setDevice("0", "DS3232");
   board->setI2CType(new I2CSema(EAPI_ID_I2C_EXTERNAL));
   for (auto i : board->getProperties()) {
      std::cout << i << '\n';
   }
   std::string x = board->read("seconds");
   cout << x << endl;
   x = board->read("temperature");
   cout << x << endl;
   board->write("SRAM1", "61K");
   x = board->read("SRAM1");
   cout << x << endl;
   return 0;
}
