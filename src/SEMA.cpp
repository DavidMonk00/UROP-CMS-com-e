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
#include "ATCABoard.h"
using namespace std;

int main(int argc, char* argv[]) {
   std::string a;
   a = argc > 1 ? argv[1] : "200K";
   I2C_base* i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   ATCABoard* board = new ATCABoard(i2c);
   board->setBus("1");
   uint8_t buffer;
   for (int i = 0; i < 0xFF; i=i+2) {
      i2c->receiveData(i,(char*)&buffer,1,0x00);
      if (buffer != 0xF0) {
         printf("Address 0x%X : 0x%X\n", i, buffer);
      }
   }
   /*board->setDevice("1","PCI Clock");
   for (auto i : board->getDevices()) { std::cout << i << '\n'; }
   for (auto i : board->getProperties()) { std::cout << i << '\n'; }
   printf("Vendor ID: 0x%X\n", atoi(board->read("vendor ID").c_str()));
   printf("Device ID: 0x%X\n", atoi(board->read("device ID").c_str()));
   std::cout << board->read("clock frequency") << '\n';
   std::cout << board->read("PLL mode") << '\n';
   std::cout << board->read("output enable") << '\n';*/
   delete board;
   return 0;
}
