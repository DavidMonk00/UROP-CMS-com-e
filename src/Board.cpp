#include "Board.h"

void Board::setBus(std::string bus) {
   i2c_bus = bus_map[bus];
}
