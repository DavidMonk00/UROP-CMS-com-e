/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include "I2C.h"

using namespace std;

#define ATCA_EEPROM 0xAE
#define ATCA_ARBITER 0xE0
#define ATCA_FANOUT 0xE4
#define ATCA_ETH_PHY 0xAC
#define ATCA_PCI_CLK 0xD8

class ATCA {
private:
   I2C_base* i2c;
   uint8_t buffer;
   bool downstream_available;
public:
   ATCA(I2C_base* i2c_ptr) {
      i2c = i2c_ptr;
      downstream_available = false;
   }
   ~ATCA(void) {
      if (downstream_available) {
         buffer = 0x00;
         i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
      }
   }
   bool checkAvailability(void) {
      i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
      return (buffer >> 1) & 0b1;
   }
   void requestBus(void) {
      buffer = 0x7b; //enable interrupt
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x05);
      buffer = 0x00; //set request time
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x03);
      buffer = 0x01; //request downstream
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
      if (checkAvailability()) {
         buffer = 0x05; //enable downstream
         i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
         downstream_available = true;
      } else {
         std::cout << "Bus not available." << '\n';
         downstream_available = false;
         exit(-1);
      }
   }
   void setFanOut(uint8_t buses) {
      buffer = buses;
      i2c->sendData(ATCA_FANOUT, (char*)&buffer, 1, 0x00);
   }
   void printSynthRegisters(void) {
      for (uint8_t i = 0; i < 8; i++) {
          buffer = 0x00;
          i2c->receiveData(ATCA_PCI_CLK, (char*)&buffer, 1, i|0b10000000);
          printf("PCI clock byte %d : 0x%X\n", i, buffer);
      }
   }
   void giveUpBus(void) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
      downstream_available = false;
   }
};

int main(int argc, char* argv[]) {
   uint8_t buffer = 0;
   I2CSema* i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   ATCA* A = new ATCA(i2c);
   A->requestBus();
   A->setFanOut(0b00000010);
   //buffer = 0x00;
   //i2c->sendData(ATCA_PCI_CLK, (char*)&buffer, 1, 0|0b10000000);
   //A->printSynthRegisters();
   int N = 2;
   uint8_t* b = (uint8_t*)malloc(N*sizeof(uint8_t));
   for (int i = 0; i < 30; i++) {
      A->printSynthRegisters();
      i2c->receiveData(0b1010110, (char*)b, N, 0x00);
      for (int i = 0; i < N; i++) {
         printf("Ethernet PHY byte %d : 0x%X\n", i, b[i]);
      }
      sleep(2);
   }
   delete A;
   return 0;
}
