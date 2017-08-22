/**
  @file Update.hpp
  @brief Defines the Update class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include "CouchDB.hpp"
#include "ATCABoard.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <boost/filesystem.hpp>

class Update {
private:
   ATCABoard* board;
   json config;
   void sendFlag(json data, json metadata, Client* client);
   void writeConfig(void);
   std::string getTime(void);
public:
   Update(I2C_base* i2c);
   ~Update(void);
   void saveActive(void);
   void saveStatic(void);
   void purgeDatabase(void);
   void getConfig(void);
};
