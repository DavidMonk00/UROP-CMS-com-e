#include "ATCABoard.h"

ATCABoard::ATCABoard(void) {
   downstream_available = false;
}

ATCABoard::~ATCABoard(void) {
   if (downstream_available) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
}

bool ATCABoard::checkAvailability(void) {
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   return (buffer >> 1) & 0b1;
}

void ATCABoard::requestBus(void) {
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
