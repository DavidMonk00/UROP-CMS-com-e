#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assert.hpp>
#include <boost/typeof/std/complex.hpp>
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
#include <unordered_map>
#include <algorithm>
#include <stdlib.h>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using namespace boost::units;
using namespace boost::units::si;

typedef std::pair<int, int> pair_type;
typedef std::vector<pair_type> pairs_type;
typedef std::pair<double , pairs_type> phys_quant;

struct units_and_powers : qi::grammar<std::string::iterator, phys_quant()> {
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
   qi::rule<std::string::iterator, int()> unit_power;
   qi::rule<std::string::iterator, pairs_type()> units;
   qi::rule<std::string::iterator, phys_quant()> query;
};

/*quantity<energy> work(const quantity<force>& F, const quantity<length>& dx) {
    return F * dx;
}*/

typedef boost::variant< double , quantity<length>, quantity<mass> > units_variant;

class multiply : public boost::static_visitor<> {
public:
    units_variant mRet;
    double mDouble;

    multiply ( double aDouble ) : mDouble( aDouble ) {}
    template <typename T>
    void operator()( T & operand ) {
        mRet = operand * mDouble;
    }
};

int main() {
   phys_quant m;
   std::string units("9.81 kg m^2 s^-2");
   units_and_powers p;
   bool result = qi::parse(units.begin(), units.end(), p,  m);
   std::cout << m.first << std::endl;
   for (auto i:  m.second)
   {
      std::cout << i.first << " : " << i.second << std::endl;
   }

   units_variant l( 3.1415 );
   l = 6.1 * si::kilogram;

   multiply v(2.0);
   boost::apply_visitor( v , l );
   //l = v.mRet;
   std::cout << l << " " << v.mRet << std::endl;
   std::cout << boost::get<quantity<mass> >(l) << '\n';

   /*std::pair<double, std::vector<std::pair<std::string, int> > > value;
   value = getValue("9.81 kg m s^-2");
   for(int i = 0; i < value.second.size(); i++){
      std::cout << value.second[i].first << " to the power "<< value.second[i].second << '\n';
   }
   units_variant dx;
   U = 2.0*meter;
   quantity<force>     F(2.0 * newton); // Define a quantity of force.
   quantity<length>    dx(2.0 * meter); // and a distance,
   quantity<energy>    E(work(F,dx));  // and calculate the work done.
   std::cout << "F  = " << F << std::endl
             << "dx = " << dx << std::endl
             << "E  = " << E << std::endl
             << "U  = " << U << std::endl;*/
   return 0;
}
