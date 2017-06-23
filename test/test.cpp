#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assert.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
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

template <typename Iterator>
struct units_and_powers : qi::grammar<Iterator, std::map<std::string, std::string>()> {
   units_and_powers() : units_and_powers::base_type(query) {
        query =  pair >> *((qi::lit(';') | '&') >> pair);
        pair  =  unit >> -('^' >> power);
        unit  =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
        power = +qi::char_("a-zA-Z_0-9");
    }
    qi::rule<Iterator, std::map<std::string, std::string>()> query;
    qi::rule<Iterator, std::pair<std::string, std::string>()> pair;
    qi::rule<Iterator, std::string()> unit, power;
};

std::string parseUnit(std::string unit) {
   using qi::symbols;
   symbols<char, std::string> sym;
   sym.add
      ("m","metres")
      ("K","kelvin")
      ("s","seconds")
   ;
   std::string i = "Error";
   test_parser_attr(unit.c_str(), sym, i);
   return i;
}

void splitUnits(std::string units) {
   using qi:: char_;
   using qi:: parse;
   std::vector<std::string> unit;
   parse(units.begin(), units.end(), *(char_), unit);
   for(int i=0; i<unit.size(); ++i) {
      std::cout << unit[i] << ' ';
   }
   std::cout << std::endl;
}

int main(void) {
   using qi:: char_;
   using qi::double_;
   using qi:: parse;
   std::string t("31.5ms^-1");
   double value;
   std::string units;
   parse(t.begin(), t.end(), double_ >> *char_, value, units);
   printf("Value: %.02f\nUnits: %s\n", value, units.c_str());
   splitUnits(units);
   std::string input("s^3;m;K^2");  // input to parse
   std::string::iterator begin = input.begin();
   std::string::iterator end = input.end();
   units_and_powers<std::string::iterator> p;    // create instance of parser
   std::map<std::string, std::string> m;        // map to receive results
   bool result = qi::parse(begin, end, p, m);
   std::cout << m["s"] << '\n';   // returns true if successful
   return 0;
}
