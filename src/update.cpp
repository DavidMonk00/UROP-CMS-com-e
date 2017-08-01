#include "update.h"

Update::Update(void) {
   board = new ATCABoard("SEMA");
   server = new Server();
   std::ifstream file("bin/config.txt");
   std::string str;
   while (file >> str) {
      target.push_back(str);
   }
}

Update::~Update(void) {
   delete board;
   delete server;
}

void Update::saveActive(void) {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::ostringstream oss;
   oss << std::put_time(&tm, "%Y%m%d%H%M%S");
   std::string time_str = oss.str();
   json board_dict;
   board_dict["_id"] = time_str;
   std::ifstream data_file("bin/active.json");
   json active;
   data_file >> active;
   for (auto bus : board->getBuses()) {
      board_dict[bus] = {};
      board->setBus(bus);
      for (auto device : board->getDevices()) {
         board_dict[bus][device] = {};
         board->setDevice(device);
         for (auto property : board->getProperties()) {
            if (active[bus][device][property]) {
               board_dict[bus][device][property] = board->read(property);
            }
         }
      }
   }
   server->setDatabase("data");
   server->uploadDocument(board_dict);
   server->pushDatabase();
}

void Update::saveStatic(void) {
   server->setDatabase("data");
   json doc = server->getStaticDocument();
   bool flag = false;
   json metadata;
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::ostringstream oss;
   oss << std::put_time(&tm, "%Y%m%d%H%M%S");
   std::string time_str = oss.str();
   json board_dict;
   board_dict["_id"] = doc["_id"];
   board_dict["_rev"] = doc["_rev"];
   std::ifstream data_file("bin/active.json");
   json active;
   data_file >> active;
   for (auto bus : board->getBuses()) {
      board_dict[bus] = {};
      board->setBus(bus);
      for (auto device : board->getDevices()) {
         board_dict[bus][device] = {};
         board->setDevice(device);
         for (auto property : board->getProperties()) {
            if (!active[bus][device][property]) {
               std::string reg = board->read(property);
               board_dict[bus][device][property] = reg;
               if (reg != doc[bus][device][property]) {
                  flag = true;
                  metadata.push_back({bus, device, property});
               }
            }
         }
      }
   }
   if (flag) {
      sendFlag(board_dict, metadata);
   }
   server->uploadDocument(board_dict);
   server->pushDatabase();
}

void Update::sendFlag(json data, json metadata) {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::ostringstream oss;
   oss << std::put_time(&tm, "%Y%m%d%H%M%S");
   std::string time_str = oss.str();
   json flag_dict = {
      {"_id",target[1] + "-" + time_str},
      {"properties", data},
      {"metadata", metadata}
   };
   server->uploadDocument(target[0]+"/flags",flag_dict);
}
