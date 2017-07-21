/**
  @file Board.h
  @brief Defines the base Board class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once

#include "I2CBus.h"
#include "parser.h"

class ToString : public boost::static_visitor<> {
public:
   std::string mRet;
   void operator() (std::string & operand ) {
      mRet = operand;
   }
   void operator() (int & operand) {
      char str[64];
      sprintf(str, "%d", operand);
      mRet = str;
   }
   template <typename T>
   void operator()( T & operand ) {
      mRet = boost::units::to_string(operand);
   }
};

class Board {
protected:
   std::unordered_map<std::string, I2CBus*> bus_map;
   I2CBus* i2c_bus;
   I2C_base* i2c;
public:
   virtual std::unordered_map<std::string, I2CBus*> getMap(void) = 0;
   void setBus(std::string bus);
   void setDevice(std::string device);
   void setDevice(std::string bus, std::string device);
   void setI2CType(I2C_base* i2c_type);
   std::vector<std::string> getBuses(void);
   std::vector<std::string> getDevices(void);
   std::vector<std::string> getDevices(std::string bus);
   std::vector<std::string> getProperties(void);
   std::vector<std::string> getProperties(std::string device);
   std::vector<std::string> getProperties(std::string bus, std::string device);
   std::string read(std::string property);
   void write(std::string property, std::string value);
};
