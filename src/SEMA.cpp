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
#include "diagnostics.h"

using namespace std;

#define ATCA_EEPROM 0xAE
#define ATCA_ARBITER 0xE0

int main(int argc, char* argv[]) {
   I2CSema* i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   cout << i2c->getBusCap() << endl;
   uint8_t buffer;
   printf("Addr : 0x%X\n", ATCA_EEPROM);
   for (int i = 0; i < 16; i++) {
      buffer = 2*i;
      i2c->sendData(ATCA_EEPROM, (char*)&buffer, 1, i);
      buffer = 0;
      i2c->receiveData(ATCA_EEPROM, (char*)&buffer, 1, i);
      printf("%d : %X\n", i, buffer);
   }
   printf("Addr : 0x%X\n", ATCA_ARBITER);
   buffer = 0x7b;
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x05);
   buffer = 0x64;
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x03);
   buffer = 0x01;
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   for (int i = 0; i < 0b111; i++) {
      uint8_t buffer = 0;
      i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, i);
      printf("%d : %X\n", i, buffer);
   }
   printf("\n");
   return 0;
}
