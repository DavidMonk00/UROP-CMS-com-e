/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk
  @version 1.0
*/

#include "Measurement.h"
#include "parser.h"
#include <string>
using namespace std;

int main() {
  Measurement M = Measurement("DS3232");
  M.printProperties();
  string s = M.read("hours");
  printf("%s\n", s.c_str());
  Parser P;
  std::pair<double, std::vector<std::pair<std::string, int> > > value;
  value = P.getValue("9.81 kg m s^-2");
  for(int i = 0; i < value.second.size(); i++){
     std::cout << value.second[i].second << '\n';
  }
  return 0;
}
