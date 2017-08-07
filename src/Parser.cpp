/**
  @file Parser.cpp
  @brief Defines functions for the Parser class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "parser.hpp"

/**
   @brief Parse string input into scaled double value and integer representation of unit.
   @param input - String input.
   @return Pair of double and integer.
*/
std::pair<double,int> Parser::getQuantityPair(std::string input) {
   std::pair<double, int> quantity;
   using qi:: char_;
   using qi::double_;
   using qi:: parse;
   std::string units;
   using boost::spirit::qi::symbols;
   symbols<char, int> unit_symbols;
   unit_symbols.add
      ("%", 1)
      ("V", 2)
      ("A", 3)
      ("s", 4)
      ("Hz",5)
      ("K", 6)
      ("b/s",7)
   ;
   parse(input.begin(), input.end(), double_ >> *char_, quantity.first, units);
   if (units.length() == 0) {
      quantity.second = 0;
      return quantity;
   } else {
      std::vector<char> pre = {'p','n','u','m','k','M','G','T','P','E'};
      auto lambda = [](std::string s, std::vector<char> pre){bool in = false;for (auto i : pre) {if (s.c_str()[0] == i) {in = true;}};return in;};
      if (lambda(units, pre)) {
         symbols<char, int> sym;
         sym.add
            ("p", -12)
            ("n", -9)
            ("u", -6)
            ("m", -3)
            ("k", 3)
            ("M", 6)
            ("G", 9)
            ("T", 12)
            ("P", 15)
            ("E", 18)
         ;
         int i;
         test_parser_attr(units.substr(0,1).c_str(), sym, i);
         quantity.first = quantity.first*std::pow(10,i);
         test_parser_attr(units.substr(1, units.length()).c_str(), unit_symbols, quantity.second);
      } else {
         test_parser_attr(units.c_str(), unit_symbols, quantity.second);
      }
      return quantity;
   }
}

/**
   @brief Parse string input into boost::units quantity.
   @param input - String input.
   @return Value with associated unit.
*/
units_variant Parser::getQuantity(std::string input) {
   std::pair<double,int> pair = getQuantityPair(input);
   boost::variant<double,
                  quantity<electric_potential>,
                  quantity<current>,
                  quantity<si::time>,
                  quantity<frequency>,
                  quantity<temperature> > var;
   switch (pair.second) {
      case 2:
         var = pair.first*volts;
         break;
      case 3:
         var = pair.first*amperes;
         break;
      case 4:
         var = pair.first*seconds;
         break;
      case 5:
         var = pair.first*hertz;
         break;
      case 6:
         var = pair.first*kelvin;
         break;
      case 1:
      default:
         var = pair.first;
   }
   return var;
}

/**
   @brief Get number as value and vector of pairs of strings.
   @param input - String of value and units.
   @return phys_quant type containing value and units.
*/
phys_quant LegacyParser::getQuantity(std::string input) {
  phys_quant m;
  units_and_powers<std::string::iterator> p;
  bool result = qi::parse(input.begin(), input.end(), p,  m);
  return m;
}
