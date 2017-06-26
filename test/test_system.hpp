// Boost.Units - A C++ library for zero-overhead dimensional analysis and
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MCS_TEST_SYSTEM_HPP
#define MCS_TEST_SYSTEM_HPP

#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/units/base_dimension.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/io.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/unit.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/make_system.hpp>

namespace boost {
namespace units {

//[test_system_snippet_3
typedef length_base_dimension::dimension_type    length_dimension;
typedef mass_base_dimension::dimension_type      mass_dimension;
typedef time_base_dimension::dimension_type      time_dimension;
//]

//[test_system_snippet_5
typedef derived_dimension<length_base_dimension,2>::type area_dimension;
typedef derived_dimension<mass_base_dimension,1,
                          length_base_dimension,2,
                          time_base_dimension,-2>::type energy_dimension;
//]

namespace test {

//[test_system_snippet_6

struct meter_base_unit : base_unit<meter_base_unit, length_dimension, 1> { };
struct kilogram_base_unit : base_unit<kilogram_base_unit, mass_dimension, 2> { };
struct second_base_unit : base_unit<second_base_unit, time_dimension, 3> { };

typedef make_system<
    meter_base_unit,
    kilogram_base_unit,
    second_base_unit>::type mks_system;

/// unit typedefs
typedef unit<dimensionless_type,mks_system>      dimensionless;

typedef unit<length_dimension,mks_system>        length;
typedef unit<mass_dimension,mks_system>          mass;
typedef unit<time_dimension,mks_system>          time;

typedef unit<area_dimension,mks_system>          area;
typedef unit<energy_dimension,mks_system>        energy;
//]

//[test_system_snippet_7
/// unit constants
BOOST_UNITS_STATIC_CONSTANT(meter,length);
BOOST_UNITS_STATIC_CONSTANT(meters,length);
BOOST_UNITS_STATIC_CONSTANT(kilogram,mass);
BOOST_UNITS_STATIC_CONSTANT(kilograms,mass);
BOOST_UNITS_STATIC_CONSTANT(second,time);
BOOST_UNITS_STATIC_CONSTANT(seconds,time);

BOOST_UNITS_STATIC_CONSTANT(square_meter,area);
BOOST_UNITS_STATIC_CONSTANT(square_meters,area);
BOOST_UNITS_STATIC_CONSTANT(joule,energy);
BOOST_UNITS_STATIC_CONSTANT(joules,energy);
//]

} // namespace test

//[test_system_snippet_8
template<> struct base_unit_info<test::meter_base_unit>
{
    static std::string name()               { return "meter"; }
    static std::string symbol()             { return "m"; }
};
//]

template<> struct base_unit_info<test::kilogram_base_unit>
{
    static std::string name()               { return "kilogram"; }
    static std::string symbol()             { return "kg"; }
};

template<> struct base_unit_info<test::second_base_unit>
{
    static std::string name()               { return "second"; }
    static std::string symbol()             { return "s"; }
};

} // namespace units

} // namespace boost

#endif // MCS_TEST_SYSTEM_HPP
