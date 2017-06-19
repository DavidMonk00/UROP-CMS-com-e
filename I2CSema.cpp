#include "I2CSema.h"

//Initialise SEMA library
I2CSema::I2CSema(void) {
  //IP address is localhost
  char ipAddr[24] = "127.0.0.1";
  uint32_t ret = 0;
  ret = SemaEApiLibInitialize(false, IP_V4, ipAddr, 0, (char*)"123", &handler);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("Can't initialise library. Error code: %X\n", ret);
  }
  //Check I2C availibilty through the Diagnostics class
  printf("Initialised library. Checking I2C status...\n");
  id = EAPI_ID_I2C_EXTERNAL;
  Diagnostics d = Diagnostics(handler);
  //d.printI2CSupport();
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

char* I2CSema::receiveData(char* buffer) {
  uint32_t addr = 0b1101000;
  uint32_t Cmd = 0x00;
  uint32_t ret = 0;
  uint32_t BufLen = sizeof(buffer)/(sizeof(char));
  uint32_t ByteCnt = BufLen;
  ret = SemaEApiI2CReadTransfer(handler, id, addr, Cmd, (void*)buffer, BufLen, ByteCnt);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    return buffer;
  }
}

void I2CSema::sendData(char* buffer) {
  uint32_t addr = 0b1101000;
  uint32_t Cmd = 0x20;
  uint32_t ret = 0;
  uint32_t BufLen = sizeof(buffer)/(sizeof(char));
  uint32_t ByteCnt = BufLen;
  ret = SemaEApiI2CWriteTransfer(handler, id, addr, Cmd, (void*)buffer, ByteCnt);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  }
}
