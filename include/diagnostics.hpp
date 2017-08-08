/**
  diagnostics.h
  Purpose: defines SEMA Diagnostics class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <Sema.h>
using namespace std;

/**
  SEMA diagnostics class. Displays useful information about the board.
*/
class Diagnostics {
private:
   bool newHandler;
   uint32_t handler;
   void getBoardInfo(EApiId_t id, string message);
   void getBoardTemp(EApiId_t id, string message);
   void getBoardCapabilitiesHex(EApiId_t id, string message);
public:
   Diagnostics(void);
   Diagnostics(uint32_t h);
   ~Diagnostics();
   void printBoardInfo(void);
   void printBoardTemperatures(void);
   void printI2CSupport(void);
};
