/**
  @file ATCABoard.h
  @brief Defines the derived ATCABoard class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include "Board.h"

#define ATCA_ARBITER 0xE0
#define ATCA_FANOUT 0xE4

class ATCABoard : public Board {
private:
   uint8_t buffer;
   bool downstream_available;
   bool i2c_set;
   bool checkAvailability(void);
   void requestBus(void); //TODO add argument for arbiter timeout. Current default is infinity.
   void setFanOut(uint8_t buses);
public:
   ATCABoard(I2C_base* i2c_type);
   ATCABoard(std::string i2c_string);
   ~ATCABoard(void);
   std::unordered_map<std::string, I2CBus*> getMap(void);
   void setBus(std::string bus);
   void setDevice(std::string device);
   void setDevice(std::string bus, std::string device);
};
