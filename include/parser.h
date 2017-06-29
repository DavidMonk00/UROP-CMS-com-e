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
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assert.hpp>
#include <boost/typeof/std/complex.hpp>
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/systems/si/energy.hpp>
#include <boost/units/systems/si/force.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/electric_potential.hpp>
#include <boost/units/systems/si/current.hpp>
#include <boost/units/systems/si/resistance.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <complex>
#include <stdlib.h>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using namespace boost::units;
using namespace boost::units::si;

//Type definitions
typedef std::pair<int, int> pair_type;
typedef std::vector<pair_type> pairs_type;
typedef std::pair<double , pairs_type> phys_quant;
typedef boost::variant< double , quantity<length>, quantity<mass> > units_variant;

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
   Simple class to call boost::spirit parser to convert string to value and units.
*/
class Parser {
public:
  phys_quant getQuantity(std::string input);
};
