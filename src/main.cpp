/**
  main.cpp
  Purpose: main function file for testing SEMA I2C connection.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "update.h"
using namespace std;

void sleep5(void) {
   sleep(5);
}

void activeLoop(Update* update) {
   while (true) {
      std::cout << "Running active..." << '\n';
      std::thread t_sleep(sleep5);
      update->saveActive();
      t_sleep.join();
   }
}

void configLoop(Update* update) {
   while (true) {
      std::cout << "Running config..." << '\n';
      std::thread t_sleep(sleep5);
      update->getConfig();
      t_sleep.join();
   }
}

void staticLoop(Update* update) {
   while (true) {
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);
      std::ostringstream oss;
      oss << std::put_time(&tm, "%M");
      int mins = atoi(oss.str().c_str());
      if (!mins) {
         std::cout << "Running static..." << '\n';
         update->saveStatic();
         sleep(70);
      } else if (mins == 30) {
         std::cout << "Purging database..." << '\n';
         update->purgeDatabase();
         sleep(70);
      } else {
         sleep(60);
      }
   }
}

int main(int argc, char* argv[]) {
   std::cout << "Initialising couchdb..." << '\n';
   std::system("couchdb &");
   sleep(1);
   std::cout << "Initialising..." << '\n';
   curl_global_init(CURL_GLOBAL_DEFAULT);
   std::cout << "Creating object..." << '\n';
   Update* update = new Update();
   std::cout << "Creating threads..." << '\n';
   std::thread t_active(activeLoop, update);
   sleep(1);
   std::thread t_config(configLoop, update);
   sleep(1);
   std::thread t_static(staticLoop, update);
   t_active.join();
   t_config.join();
   t_static.join();
   delete update;
}
