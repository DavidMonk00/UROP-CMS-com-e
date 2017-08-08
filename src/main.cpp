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

void setGlobalConfigs(Server* server) {
   json global;
   std::ifstream global_file("/home/david/Python/urop/bin/global.json");
   global_file >> global;
   for (json::iterator bus_it = global.begin(); bus_it != global.end(); ++bus_it) {
      std::string bus = bus_it.key();
      for (json::iterator device_it = bus_it.value().begin(); device_it != bus_it.value().end(); ++device_it) {
         std::string device = device_it.key();
         for (json::iterator property_it = device_it.value().begin(); property_it != device_it.value().end(); ++property_it) {
            std::string property = property_it.key();
            server->editConfig(bus,device,property,property_it.value().get<std::string>());
         }
      }
   }
   server->pushChanges();
}

int main(int argc, char* argv[]) {
   curl_global_init(CURL_GLOBAL_DEFAULT);
   Server* server = new Server();
   setGlobalConfigs(server);
   delete server;
}
