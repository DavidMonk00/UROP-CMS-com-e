#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assert.hpp>
#include <boost/typeof/std/complex.hpp>
#include <boost/units/systems/si/energy.hpp>
#include <boost/units/systems/si/force.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/electric_potential.hpp>
#include <boost/units/systems/si/current.hpp>
#include <boost/units/systems/si/resistance.hpp>
#include <boost/units/systems/si/io.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <complex>
#include <stdlib.h>

template <typename P, typename T>
void test_parser_attr(char const* input, P const& p, T& attr, bool full_match = true) {
   using boost::spirit::qi::parse;
   char const* f(input);
   char const* l(f + strlen(f));
   if (parse(f, l, p, attr) && (!full_match || (f == l))) {
   } else {
      std::cout << "Parse failed." << std::endl;
      std::cout << input << std::endl;
      exit(-1);
   }
}

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

typedef std::pair<std::string, std::string> pair_type;
typedef std::vector<pair_type> pairs_type;

template <typename Iterator>
struct units_and_powers : qi::grammar<Iterator, pairs_type()> {
   units_and_powers() : units_and_powers::base_type(query) {
        query =  pair >> *((qi::lit(';') | ' ') >> pair);
        pair  =  unit >> -('^' >> power);
        unit  =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
        power = +qi::char_("a-zA-Z_0-9")|qi::char_('-') >> *qi::char_("a-zA-Z_0-9");
    }
    qi::rule<Iterator, pairs_type()> query;
    qi::rule<Iterator, pair_type()> pair;
    qi::rule<Iterator, std::string()> unit;
    qi::rule<Iterator, std::string()> power;
};

std::string parseUnit(std::string unit) {
   using qi::symbols;
   symbols<char, std::string> sym;
   sym.add
      ("m","metres")
      ("K","kelvin")
      ("s","seconds")
      ("Hz","Hertz")
      ("kg","kilograms")
   ;
   std::string i = "Error";
   test_parser_attr(unit.c_str(), sym, i);
   return i;
}

std::vector<std::pair<std::string, int> > splitUnits(std::string units) {
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

std::pair<double, std::vector<std::pair<std::string, int> > > getValue(std::string input) {
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

int main(void) {
   std::pair<double, std::vector<std::pair<std::string, int> > > value;
   value = getValue("9.81 kg m s^-2");
   return 0;
}
