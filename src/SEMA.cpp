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
#define ATCA_U43 0xE2

void requestBus(I2C_base* i2c) {
   uint8_t buffer;
   buffer = 0x7b; //enable interrupt
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x05);
   buffer = 0xff; //set request time
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x03);
   buffer = 0x01; //request downstream
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   buffer = 0;
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   printf("CONTR: 0x%X\n", buffer);
   if ((buffer >> 1) & 0b1) {
      cout << "Downstream available" << endl;
   } else {
      cout << "Downstream unavailable" << endl;
   }
   buffer = 0;
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x02);
   printf("STATUS: 0x%X\n", buffer);
   buffer = 0;
   i2c->receiveData(ATCA_U43, (char*)&buffer, 1, 0x00);
   printf("Control register: %X\n", buffer);
   buffer = 0;
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   if ((buffer >> 1) & 0b1) {
      cout << "Downstream available" << endl;
   } else {
      cout << "Downstream unavailable" << endl;
   }
}

int main(int argc, char* argv[]) {
   I2CSema* i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
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
   requestBus(i2c);
   printf("\n");
   return 0;
}
