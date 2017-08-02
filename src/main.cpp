/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include "I2C.h"
#include "ATCABoard.h"
#include "couchdb.h"
#include "update.h"
using namespace std;

void SEMA(void) {
   I2C_base* i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   ATCABoard* board = new ATCABoard(i2c);
   board->setBus("one");
   uint8_t buffer;
   for (int i = 0; i < 0xFF; i=i+2) {
      i2c->receiveData(i,(char*)&buffer,1,0x00);
      if (buffer != 0xF0) {
         printf("Address 0x%X : 0x%X\n", i, buffer);
      }
   }
   delete board;
}

int main(int argc, char* argv[]) {
   Update* update = new Update();
   update->saveActive();
   update->saveStatic();
   update->purgeDatabase();
   update->getConfig();
   delete update;
}
