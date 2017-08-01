#pragma once
#include "couchdb.h"
#include "ATCABoard.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <fstream>

class Update {
private:
   ATCABoard* board;
   Server* server;
public:
   Update(void);
   ~Update(void);
   void saveActive(void);
};
