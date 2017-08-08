/**
  @file Board.hpp
  @brief Defines the base Board class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once

#include "I2CBus.hpp"
#include "parser.hpp"

class ToString : public boost::static_visitor<> {
private:
   std::string convertString(std::string str) {
      int index = str.find(" ")+1;
      std::string unit;
      char s[64];
      if (index) {
         unit = str.substr(str.find(" ")+1);
      } else {
         sprintf(s,"%.014f",std::stof(str));
         str = s;
         return str;
      }

      float x = std::stof(str);
      int p = ((int)log10(x))/3;
      float val = x/pow(1000,p);

      switch (p) {
         case -4:
            sprintf(s,"%.02f p", val);
            break;
         case -3:
            sprintf(s,"%.02f n", val);
            break;
         case -2:
            sprintf(s,"%.02f u", val);
            break;
         case -1:
            sprintf(s,"%.02f m", val);
            break;
         case 0:
            sprintf(s,"%.02f ", val);
            break;
         case 1:
            sprintf(s,"%.02f k", val);
            break;
         case 2:
            sprintf(s,"%.02f M", val);
            break;
         case 3:
            sprintf(s,"%.02f G", val);
            break;
         case 4:
            sprintf(s,"%.02f T", val);
            break;
         case 5:
            sprintf(s,"%.02f P", val);
            break;
         case 6:
            sprintf(s,"%.02f E", val);
            break;
      }
      str = s + unit;
      return str;
   }
public:
   std::string mRet;
   void operator() (std::string & operand ) {
      mRet = operand;
   }
   void operator() (int & operand) {
      if (operand == 0xF0) {
         mRet = "Register not available.";
      } else {
         char str[64];
         sprintf(str, "%d", operand);
         mRet = str;
      }
   }
   template <typename T>
   void operator()( T & operand ) {
      mRet = convertString(boost::units::to_string(operand));
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
