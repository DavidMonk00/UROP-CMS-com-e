#include "parser.h"

phys_quant Parser::getQuantity(std::string input) {
  phys_quant m;
  std::string units("9.81 kg m^2 s^-2");
  units_and_powers<std::string::iterator> p;
  bool result = qi::parse(units.begin(), units.end(), p,  m);
  std::cout << m.first << std::endl;
  for (auto i:  m.second) {
     std::cout << i.first << " : " << i.second << std::endl;
  }
  return m;
}
