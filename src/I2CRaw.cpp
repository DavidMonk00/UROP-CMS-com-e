/**
  @file I2CRaw.cpp
  @brief Defines functions for the I2CRaw class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CRaw.hpp"

/**
   @brief Class constructor.
*/
I2CRaw::I2CRaw() {
   i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   requestBus();
   setFanOut(0b00000100);
}

/**
   @brief Class destructor.
*/
I2CRaw::~I2CRaw() {
   if (checkAvailability()) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
   delete i2c;
}

/**
   @brief Check availability of downstream I2C bus.
   @return True if available, false if not.
*/
bool I2CRaw::checkAvailability(void) {
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   return (buffer >> 1) & 0b1;
}

/**
   @brief Request downstream I2C bus from arbiter.
*/
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
      printf("Bus not available.\n"); //TODO throw exception instead?
      exit(-1);
   }
}

/**
   @brief Set I2C fanout.
   @param buses - Each bit is set to one if bus for downstream to be available. Example: 0b00000010 allows only bus 1 to be available.
*/
void I2CRaw::setFanOut(uint8_t buses) {
   buffer = buses;
   i2c->sendData(ATCA_FANOUT, (char*)&buffer, 1, 0x00);
}

/**
   @brief Read from register.
   @param address - Device address.
   @param start_point - Internal register address
   @return Integer representation of hex output.
*/
int I2CRaw::read(uint32_t address, uint32_t start_point) {
   i2c->receiveData(address, (char*)&buffer, 1, start_point);
   return (int)buffer;
}

/**
   @brief Write to register.
   @param address - Device address.
   @param buffer - Data to be written to register.
   @param start_point - Internal register address
*/
void I2CRaw::write(uint32_t address, uint8_t buffer, uint32_t start_point) {
   i2c->sendData(address, (char*)&buffer, 1, start_point);
}
