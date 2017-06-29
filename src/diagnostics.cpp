/**
  diagnostics.cpp
  Purpose: defines functions for the Diagnostics class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "diagnostics.h"

/**
  Constructor function if library has yet to initialised.
*/
Diagnostics::Diagnostics(void) {
  //IP address is localhost
  char ipAddr[24] = "127.0.0.1";
  uint32_t ret = 0;
  ret = SemaEApiLibInitialize(false, IP_V4, ipAddr, 0, (char*)"123", &handler);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("Can't initialise library. Error code: %X\n", ret);
  }
}

/**
  Constructor function if library has already been initialised but diagnostics
  need to be performed.
*/
Diagnostics::Diagnostics(uint32_t h) {
  handler = h;
}

/**
  Class destructor.
*/
Diagnostics::~Diagnostics(void) {
}

/**
  Print selcted information about the COM-e board.
  Current information printed: Board Manufacturer and Board Name.
*/
void Diagnostics::printBoardInfo(void) {
  printf("\n******** BOARD INFORMATION ********\n");
  getBoardInfo(EAPI_ID_BOARD_MANUFACTURER_STR, "Board Manufacturer");
  getBoardInfo(EAPI_ID_BOARD_NAME_STR, "Board Name");
}

/**
  Print current board and CPU temperatures.
*/
void Diagnostics::printBoardTemperatures(void) {
  printf("\n******** TEMPERATURES ********\n");
  getBoardTemp(EAPI_ID_HWMON_SYSTEM_TEMP, "System Temperature");
  getBoardTemp(EAPI_ID_HWMON_CPU_TEMP, "CPU Temperature");
}

/**
  Print available I2C buses.
*/
void Diagnostics::printI2CSupport(void) {
  printf("\n******** I2C Status ********\n");
  uint32_t ret = 0;
  uint32_t buf;
  ret = SemaEApiBoardGetValue(handler, SEMA_EAPI_ID_BOARD_CAPABILITIES, &buf);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    if (buf && 0x10000) {
      printf("Ext. I2C #1 bus available\n");
    };
    if (buf && 0x20000) {
      printf("Ext. I2C #2 bus available\n");
    };
    if (buf && 0x20000000) {
      printf("Ext. I2C #3 bus available\n");
    };
    if (buf && 0x40000000) {
      printf("Ext. I2C #4 bus available\n");
    };
  }
}

//Private functions
/**
  Get individual board strings.
  @param id - ID for board values
  @param message - text to display before value
*/
void Diagnostics::getBoardInfo(EApiId_t id, string message) {
  char buffer[256] = {};
  uint32_t ret = 0;
  uint32_t buf_len = 256;
  printf("%s: ", message.c_str());
  ret = SemaEApiBoardGetStringA(handler, id, buffer, &buf_len);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    printf("'%s'", buffer);
  }
  printf("\n");
}

/**
  Get individual temperatures.
  @param id - ID for board values
  @param message - text to display before value
*/
void Diagnostics::getBoardTemp(uint32_t id, string message) {
  uint32_t ret = 0;
  uint32_t buf;
  printf("%s: ", message.c_str());
  ret = SemaEApiBoardGetValue(handler, id, &buf);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    float temp;
    temp = (float)buf/10;
    printf("%.01fK\n", temp);
  }
}

/**
  DEPRECATED
  Get the hexadicemal value of the board capabilities.
  @param id - ID for board values
  @param message - text to display before value
*/
void Diagnostics::getBoardCapabilitiesHex(uint32_t id, string message) {
  uint32_t ret = 0;
  uint32_t buf;
  printf("%s: ", message.c_str());
  ret = SemaEApiBoardGetValue(handler, id, &buf);
  if (ret != EAPI_STATUS_SUCCESS) {
    printf("ERROR: 0x%X", ret);
  } else {
    printf("%X\n", buf);
  }
}
