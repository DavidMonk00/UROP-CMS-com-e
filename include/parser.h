/**
  parser.h
  Purpose: defines Parser class.
  @author David Monk
  @version 1.0
*/

#pragma once
#include <string>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include "units_define.h"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using namespace boost::units;
using namespace boost::units::si;

//Type definitions
typedef std::pair<int, int> pair_type;
typedef std::vector<pair_type> pairs_type;
typedef std::pair<double , pairs_type> phys_quant;

/**
  Parser for symbol parsing.
*/
template <typename P, typename T>
void test_parser_attr(
    char const* input, P const& p, T& attr)
{
    using boost::spirit::qi::parse;
    char const* f(input);
    char const* l(f + strlen(f));
    parse(f, l, p, attr);
}

/**
  Template for parser to convert into value and units.
  @param Iterator - input, generally std::string::iterator
*/
template <typename Iterator>
struct units_and_powers : qi::grammar<Iterator, phys_quant()> {
   units_and_powers() : units_and_powers::base_type(query) {
      unit.add
         ("m",1)
         ("K",2)
         ("s",3)
         ("Hz",4)
         ("kg",5);

      unit_power = qi::lit('^') >> qi::int_|qi::attr(1);
      units =  (unit >> -unit_power) % (+qi::lit(" "));
      query = qi::double_ >> +(qi::lit(" ")) >> units;
   }
   qi::symbols<char, int> unit;
   qi::rule<Iterator, int()> unit_power;
   qi::rule<Iterator, pairs_type()> units;
   qi::rule<Iterator, phys_quant()> query;
};

class Parser {
private:
   std::pair<double, int> getQuantityPair(std::string input);
public:
   units_variant getQuantity(std::string input);
};

/**
   DEPRECATED - Simple class to call boost::spirit parser to convert string to value and units.
   Kept because of neat parser template.
*/
class LegacyParser {
public:
  phys_quant getQuantity(std::string input);
};
