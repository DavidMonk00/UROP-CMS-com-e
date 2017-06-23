#include "parser.h"

std::string Parser::parseUnit(std::string unit) {
   using qi::symbols;
   symbols<char, std::string> sym;
   sym.add
      ("m","metres")
      ("K","kelvin")
      ("s","seconds")
      ("Hz","Hertz")
      ("kg","kilograms")
      ("J", "joules")
   ;
   std::string i = "Error";
   test_parser_attr(unit.c_str(), sym, i);
   return i;
}

std::vector<std::pair<std::string, int> > Parser::splitUnits(std::string units) {
   std::string::iterator begin = units.begin();
   std::string::iterator end = units.end();
   units_and_powers<std::string::iterator> p;
   pairs_type m;
   bool result = qi::parse(begin, end, p, m);
   std::vector<std::pair<std::string, int> > u;
   for(int i = 0; i < m.size(); i++){
      if (m[i].second == "") {m[i].second = "1";}
      u.push_back({m[i].first, atoi(m[i].second.c_str())});
   }
   return u;
}

std::pair<double, std::vector<std::pair<std::string, int> > > Parser::getValue(std::string input) {
   std::pair<double, std::vector<std::pair<std::string, int> > > value;
   using qi:: char_;
   using qi::double_;
   using qi:: parse;
   std::string units;
   parse(input.begin(), input.end(), double_ >> " " >> *char_, value.first, units);
   printf("Value: %.02f\nUnits: %s\n", value.first, units.c_str());
   value.second = splitUnits(units);
   return value;
}
