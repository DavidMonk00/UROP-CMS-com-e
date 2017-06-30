/**
  parser.h
  Purpose: defines Parser class.
  @author David Monk
  @version 1.0
*/

#pragma once
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/assert.hpp>
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/systems/si/electric_potential.hpp>
#include <boost/units/systems/si/current.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/variant.hpp>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdlib.h>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using namespace boost::units;
using namespace boost::units::si;

//Type definitions
typedef std::pair<int, int> pair_type;
typedef std::vector<pair_type> pairs_type;
typedef std::pair<double , pairs_type> phys_quant;
typedef boost::variant<double,
                       quantity<length>,
                       quantity<boost::units::si::time>,
                       quantity<temperature> > units_variant;

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

/**
   DEPRECATED - Simple class to call boost::spirit parser to convert string to value and units.
   Kept because of neat parser template.
*/
class LegacyParser {
public:
  phys_quant getQuantity(std::string input);
};
