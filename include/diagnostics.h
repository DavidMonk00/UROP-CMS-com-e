/**
  diagnostics.h
  Purpose: defines Diagnostics class.
  @author David Monk
  @version 1.0
*/

#pragma once
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <Sema.h>
using namespace std;

/**
  SEMA diagnostics class. Displays useful information about the board.
*/
class Diagnostics {
private:
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