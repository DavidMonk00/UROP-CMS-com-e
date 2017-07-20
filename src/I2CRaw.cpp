#include "I2CRaw.h"

I2CRaw::I2CRaw() {
   i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   requestBus();
   setFanOut(0b00000010);
}

I2CRaw::~I2CRaw() {
   if (checkAvailability()) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
   delete i2c;
}

bool I2CRaw::checkAvailability(void) {
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   return (buffer >> 1) & 0b1;
}

void I2CRaw::requestBus(void) {
   buffer = 0x7b; //enable interrupt
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x05);
   buffer = 0x00; //set request time
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x03);
   buffer = 0x01; //request downstream
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   if (checkAvailability()) {
      buffer = 0x05; //enable downstream
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   } else {
      std::cout << "Bus not available." << '\n';
      exit(-1);
   }
}

void I2CRaw::setFanOut(uint8_t buses) {
   buffer = buses;
   i2c->sendData(ATCA_FANOUT, (char*)&buffer, 1, 0x00);
}

int I2CRaw::read(uint32_t address, uint32_t start_point) {
   i2c->receiveData(address, (char*)&buffer, 1, start_point);
   return (int)buffer;
}

void I2CRaw::write(uint32_t address, uint8_t buffer, uint32_t start_point) {
   i2c->sendData(address, (char*)&buffer, 1, start_point);
}
