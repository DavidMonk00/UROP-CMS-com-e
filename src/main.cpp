/**
  main.cpp
  Purpose: main function file.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "update.hpp"

void sleep5(void) {
   sleep(2);
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
      if (mins == 0) {
         std::cout << "Running static..." << '\n';
         update->saveStatic();
         sleep(65);
      } else {
         sleep(30);
      }
   }
}

void purgeLoop(Update* update) {
   while (true) {
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);
      std::ostringstream oss;
      oss << std::put_time(&tm, "%M");
      int mins = atoi(oss.str().c_str());
      if (!(mins % 10)) {
         std::cout << "Purging database..." << '\n';
         update->purgeDatabase();
         sleep(65);
      } else {
         sleep(30);
      }
   }
}

int main(int argc, char* argv[]) {
   curl_global_init(CURL_GLOBAL_DEFAULT);
   std::cout << "Creating object..." << '\n';
   Update* update = new Update();
   std::cout << "Creating threads..." << '\n';
   std::thread t_active(activeLoop, update);
   sleep(1);
   std::thread t_config(configLoop, update);
   sleep(1);
   std::thread t_static(staticLoop, update);
   sleep(1);
   std::thread t_purge(purgeLoop, update);
   t_active.join();
   t_config.join();
   t_static.join();
   t_purge.join();
   delete update;
}
