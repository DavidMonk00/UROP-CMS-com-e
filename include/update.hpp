#pragma once
#include "couchdb.hpp"
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
public:
   Update(void);
   ~Update(void);
   void saveActive(void);
   void saveStatic(void);
   void purgeDatabase(void);
   void getConfig(void);
};
