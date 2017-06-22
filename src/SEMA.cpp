/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk
  @version 1.0
*/

#include "I2C.h"
#include "Measurement.h"
#include <string>
using namespace std;

int main() {
  Measurement M = Measurement("DS3232");
  M.printProperties();
  string s = M.read("hours");
  printf("%s\n", s.c_str());
  return 0;
}
