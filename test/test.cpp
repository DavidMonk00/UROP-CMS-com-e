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

template <typename P, typename T>
void test_parser_attr(
    char const* input, P const& p, T& attr, bool full_match = true)
{
    using boost::spirit::qi::parse;

    char const* f(input);
    char const* l(f + strlen(f));
    parse(f, l, p, attr);
}

typedef std::pair<int, int> pair_type;
typedef std::vector<pair_type> pairs_type;
typedef std::pair<double , pairs_type> phys_quant;
typedef make_scaled_unit<si::time, scale<10, static_rational<-9> > >::type nanosecond;

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

typedef boost::variant< double,
                        quantity<length>,
                        quantity<mass>,
                        quantity<si::time>
                         > units_variant;

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

std::pair<double,int> foo(std::string input) {
   std::pair<double, int> number;
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
   parse(input.begin(), input.end(), double_ >> *char_, number.first, units);
   if (units.length() == 0) {
      number.second = 0;
      return number;
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
         number.first = number.first*std::pow(10,i);
         test_parser_attr(units.substr(1, units.length()).c_str(), unit_symbols, number.second);
      } else {
         test_parser_attr(units.c_str(), unit_symbols, number.second);;
      }
      return number;
   }
}

class Test {
public:
  Test(int x) {
    std::cout << x << '\n';
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
   std::cout << l << " " << v.mRet << std::endl;
   std::cout << boost::get<quantity<mass> >(l) << '\n';

   auto lambda = [](auto x, auto y){return x*y;};

   std::vector<units_variant> vec = {1.0*kilograms,1.0*meters,1.0*seconds};
   std::vector<int> powers = {1,1,2};
   auto x = 3.14159;
   auto y = lambda(x,kilograms);
   printf("%d\n", powers[0]);
   auto z = x * pow<1>(kilograms) * pow<2>(meters) * pow<-3>(seconds);
   std::cout << y << '\n';
   std::cout << 2.0*amperes << '\n';

   std::string s("MHz");
   std::vector<char> pre = {'p','n','u','m','k','M','G','T','P','E'};
   auto lambda1 = [](std::string s, std::vector<char> pre){bool in = false;for (auto i : pre) {if (s.c_str()[0] == i) {in = true;}};return in;};
   if (lambda1(s, pre)) {
      std::cout << s.substr(1,s.length()) << '\n';
   } else {
      std::cout << s << '\n';
   }

   std::string str("2.1K");
   std::pair<double,int> pair = foo(str);
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
   std::cout << var << std::endl;

   enum tTest {SEMA, FTDI, PCIe};
   tTest en = SEMA;
   switch (en) {
      case SEMA:
         std::cout << "SEMA" << '\n';
         Test T = Test(2);
         break;
      default:
         std::cout << "Something else" << '\n';
   }
   return 0;
}
