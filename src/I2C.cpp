/**
  @file I2C.cpp
  @brief Defines functions for the derived I2CSema class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2C.h"

/**
  @brief Constructor function.
  @param ID - I2C bus ID.
  @param address - Address of slave module.
*/
I2CSema::I2CSema(uint32_t ID) {
  //IP address is localhost
  char ipAddr[24] = "127.0.0.1";
  uint32_t ret = 0;
  ret = SemaEApiLibInitialize(false, IP_V4, ipAddr, 0, (char*)"123", &handler);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("Can't initialise library. Error code: %X\n", ret);
  }
  //Set I2C addresses
  id = ID;
}

/**
  @brief Constructor function with additional diagnostics.
  @param ID - I2C bus ID.
  @param address - Address of slave module.
  @param diag - Set to true to display diagnostics.
*/
I2CSema::I2CSema(uint32_t ID, uint32_t address, bool diag) {
  //IP address is localhost
  char ipAddr[24] = "127.0.0.1";
  uint32_t ret = 0;
  ret = SemaEApiLibInitialize(false, IP_V4, ipAddr, 0, (char*)"123", &handler);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("Can't initialise library. Error code: %X\n", ret);
  }
  //Check I2C availibilty through the Diagnostics class
  diagnostics = new Diagnostics(handler);
  if (diag) {
    printf("Initialised library. Checking I2C status...\n");
    diagnostics->printI2CSupport();
    printf("\n");
  }
  //Set I2C addresses
  id = ID;
  addr = address;
}

/**
  @brief Class destructor.
*/
I2CSema::~I2CSema(void) {
  SemaEApiLibUnInitialize(handler);
}

/**
  @brief Get the maximum block length for given I2C bus ID.
  @return Block length as integer.
*/
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

/**
  @brief Receive data data from addressed slave.
  @param buffer - Pointer to byte array where data will be written.
  @param bytecnt - Length of buffer.
  @param start_point - Starting address within slave.
  @return Pointer to array where data was written.
*/
void I2CSema::receiveData(uint32_t address, char* buffer, uint32_t bytecnt, uint32_t start_point) {
  uint32_t Cmd = start_point;
  uint32_t ret = 0;
  uint32_t BufLen = bytecnt;
  ret = SemaEApiI2CReadTransfer(handler, id, address, Cmd, (void*)buffer, BufLen, bytecnt);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  }
}

/**
  @brief Send data data to addressed slave.
  @param buffer - Pointer to byte array where data will be sent from.
  @param bytecnt - Length of buffer.
  @param start_point - Starting address within slave.
*/
void I2CSema::sendData(uint32_t address, char* buffer, uint32_t bytecnt, uint32_t start_point) {
  uint32_t Cmd = start_point;
  uint32_t ret = 0;
  ret = SemaEApiI2CWriteTransfer(handler, id, address, Cmd, (void*)buffer, bytecnt);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  }
}
