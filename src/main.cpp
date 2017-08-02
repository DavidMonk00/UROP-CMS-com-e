/**
  SEMA.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include "update.h"
using namespace std;

int main(int argc, char* argv[]) {
   curl_global_init(CURL_GLOBAL_DEFAULT);
   Update* update = new Update();
   if (argc == 1) {
      std::cout << "Running config..." << '\n';
      update->getConfig();
   } else {
      std::string str(argv[1]);
      if (str == "active") {
         std::cout << "Running active..." << '\n';
         update->saveActive();
      } else if (str == "purge") {
         std::cout << "Purging..." << '\n';
         update->purgeDatabase();
      } else if (str == "static") {
         std::cout << "Reading static registers..." << '\n';
         update->saveStatic();
      }
   }
   delete update;
}
