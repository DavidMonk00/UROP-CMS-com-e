#pragma once

#include <unordered_map>
#include <string>
#include "I2CBus.h"
#include "I2C_define.h"
#include "Board.h"

class COMETestBoard : public Board {
public:
   COMETestBoard(void);
   std::unordered_map<std::string, I2CBus*> getMap(void);
};
