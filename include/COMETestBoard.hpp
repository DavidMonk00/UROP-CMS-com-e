/**
  @file COMETestBoard.h
  @brief Defines the derived COMETestBoard class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once

#include "Board.hpp"

class COMETestBoard : public Board {
   bool i2c_set;
public:
   COMETestBoard(I2C_base* i2c_type);
   COMETestBoard(std::string i2c_string);
   ~COMETestBoard(void);
   std::unordered_map<std::string, I2CBus*> getMap(void);
};
