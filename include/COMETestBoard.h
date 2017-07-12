/**
  @file Board.h
  @brief Defines the derived COMETestBoard class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once

#include <unordered_map>
#include <string>
#include "I2CBus.h"
#include "I2C_define.h"
#include "Board.h"

class COMETestBoard : public Board {
public:
   COMETestBoard(void);
   COMETestBoard(I2C_base* i2c_type);
   std::unordered_map<std::string, I2CBus*> getMap(void);
};
