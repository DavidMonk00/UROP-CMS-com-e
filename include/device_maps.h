#pragma once
#include <unordered_map>
#include "I2CRegister.h"

typedef std::unordered_map<std::string, I2CBaseRegister*> register_map;

register_map DS3232_reg;
