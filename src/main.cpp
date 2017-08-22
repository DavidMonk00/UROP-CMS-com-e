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

void setConfigs(Server* server) {
   json config;
   std::ifstream config_file("/home/david/Python/urop/bin/global.json");
   config_file >> config;
   for (json::iterator bus_it = config["global"].begin(); bus_it != config["global"].end(); ++bus_it) {
      std::string bus = bus_it.key();
      for (json::iterator device_it = bus_it.value().begin(); device_it != bus_it.value().end(); ++device_it) {
         std::string device = device_it.key();
         for (json::iterator property_it = device_it.value().begin(); property_it != device_it.value().end(); ++property_it) {
            std::string property = property_it.key();
            server->editConfig(bus,device,property,property_it.value().get<std::string>());
         }
      }
   }
   for (auto board : server->getSlaves()){
      for (json::iterator bus_it = config[board["id"].get<std::string>()].begin(); bus_it != config[board["id"].get<std::string>()].end(); ++bus_it) {
         std::string bus = bus_it.key();
         for (json::iterator device_it = bus_it.value().begin(); device_it != bus_it.value().end(); ++device_it) {
            std::string device = device_it.key();
            for (json::iterator property_it = device_it.value().begin(); property_it != device_it.value().end(); ++property_it) {
               std::string property = property_it.key();
               server->editConfig(board["id"].get<std::string>(),bus,device,property,property_it.value().get<std::string>());
            }
         }
      }
   }
   server->pushChanges();
}

bool checkConfigs(Server* server) {
   bool flag = true;
   json recent = server->getRecentDocument();
   std::cout << recent << '\n';
   json config;
   std::ifstream config_file("/home/david/Python/urop/bin/global.json");
   config_file >> config;
   for (json::iterator bus_it = config["global"].begin(); bus_it != config["global"].end(); ++bus_it) {
      std::string bus = bus_it.key();
      for (json::iterator device_it = bus_it.value().begin(); device_it != bus_it.value().end(); ++device_it) {
         std::string device = device_it.key();
         for (json::iterator property_it = device_it.value().begin(); property_it != device_it.value().end(); ++property_it) {
            std::string property = property_it.key();
            if (!(atof(config["global"][bus][device][property].get<std::string>().c_str()) == atof(recent[bus][device][property].get<std::string>().c_str()))) {
               flag = false;
            }
         }
      }
   }
   for (auto board : server->getSlaves()){
      for (json::iterator bus_it = config[board["id"].get<std::string>()].begin(); bus_it != config[board["id"].get<std::string>()].end(); ++bus_it) {
         std::string bus = bus_it.key();
         for (json::iterator device_it = bus_it.value().begin(); device_it != bus_it.value().end(); ++device_it) {
            std::string device = device_it.key();
            for (json::iterator property_it = device_it.value().begin(); property_it != device_it.value().end(); ++property_it) {
               std::string property = property_it.key();
               if (!(atof(config[board["id"].get<std::string>()][bus][device][property].get<std::string>().c_str()) == atof(recent[bus][device][property].get<std::string>().c_str()))) {
                  flag = false;
               }
            }
         }
      }
   }
   return flag;
}

int main(int argc, char* argv[]) {
   curl_global_init(CURL_GLOBAL_DEFAULT);
   Server* server = new Server();
   do {
      std::cout << "Updating slave configs..." << '\n';
      setConfigs(server);
   } while(!checkConfigs(server));
   delete server;
}


//curl -X GET 'http://127.0.0.1:5984/atca001/_all_docs?limit=4&descending=true'
