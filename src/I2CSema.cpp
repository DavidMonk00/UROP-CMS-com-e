#include "I2CSema.h"

I2CSema::I2CSema(uint32_t ID, uint8_t address) {
  //IP address is localhost
  char ipAddr[24] = "127.0.0.1";
  uint32_t ret = 0;
  ret = SemaEApiLibInitialize(false, IP_V4, ipAddr, 0, (char*)"123", &handler);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("Can't initialise library. Error code: %X\n", ret);
  }
  //Set I2C addresses
  id = ID;
  addr = address;
}

//Initialise SEMA library and run diagnostics
I2CSema::I2CSema(uint32_t ID, uint8_t address, bool diag) {
  //IP address is localhost
  char ipAddr[24] = "127.0.0.1";
  uint32_t ret = 0;
  ret = SemaEApiLibInitialize(false, IP_V4, ipAddr, 0, (char*)"123", &handler);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("Can't initialise library. Error code: %X\n", ret);
  }
  //Check I2C availibilty through the Diagnostics class
  if (diag) {
    printf("Initialised library. Checking I2C status...\n");
    Diagnostics d = Diagnostics(handler);
    d.printI2CSupport();
    printf("\n");
  }
  //Set I2C addresses
  id = ID;
  addr = address;
}

//Uninitialise library within destructor
I2CSema::~I2CSema(void) {
  SemaEApiLibUnInitialize(handler);
}

/*Get the maximum block length for given I2C bus ID.*/
uint32_t I2CSema::getBusCap(void) {
  uint32_t buffer;
  uint32_t ret = 0;
  ret = SemaEApiI2CGetBusCap(handler, id, &buffer);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    return buffer;
  }
  return -1;
}

/*receive data data from addressed slave.
  Inputs:
          char* buffer - pointer to byte array where data will be written
          uint32_t bytecnt - length of buffer
          uint32_t start_point - starting address within slave
  Outputs:
          char* - returns pointer to array where data was written
*/
char* I2CSema::receiveData(char* buffer, uint32_t bytecnt, uint32_t start_point) {
  uint32_t Cmd = start_point;
  uint32_t ret = 0;
  uint32_t BufLen = bytecnt;
  ret = SemaEApiI2CReadTransfer(handler, id, addr, Cmd, (void*)buffer, BufLen, bytecnt);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    return buffer;
  }
}

/*send data data to addressed slave.
  Inputs:
          char* buffer - pointer to byte array where data will be sent from
          uint32_t bytecnt - length of buffer
          uint32_t start_point - starting address within slave
*/
void I2CSema::sendData(char* buffer, uint32_t bytecnt, uint32_t start_point) {
  uint32_t Cmd = start_point;
  uint32_t ret = 0;
  ret = SemaEApiI2CWriteTransfer(handler, id, addr, Cmd, (void*)buffer, bytecnt);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  }
}
