/**
  Parer.cpp
  Purpose: defines functions for the Parser class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "parser.h"

/**
   Get quantity as value and vector of pairs of strings.
   @param input - string of value and units
   @return pbys_quant type containing value and units
*/
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
