#include "I2C.h"
#include "Board.h"
#include <iostream>
#include <stdio.h>

class ATCABoard : public Board {
private:
   I2C_base* i2c;
   uint8_t buffer;
   bool downstream_available;
   bool checkAvailability(void);
   void requestBus(void);
public:
   ATCABoard(void);
   ~ATCABoard(void);
};

/*class ATCA {
private:
   I2C_base* i2c;
   uint8_t buffer;
   bool downstream_available;
public:
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
      i2c->sendData(ATCA_U21, (char*)&buffer, 1, 0x00);
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
};*/
