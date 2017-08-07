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

using namespace boost::units;
using namespace boost::units::si;

typedef boost::variant<double,
                       quantity<length>,
                       quantity<boost::units::si::time>,
                       quantity<temperature> > legacy_units_variant;
typedef boost::variant<std::string,
               int,
               double,
               quantity<electric_potential>,
               quantity<current>,
               quantity<si::time>,
               quantity<frequency>,
               quantity<temperature> > units_variant;
