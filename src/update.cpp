#include "update.h"

Update::Update(void) {
   board = new ATCABoard("SEMA");
   std::ifstream config_file("/root/I2C/bin/config.json");
   config_file >> config;
}

Update::~Update(void) {
   delete board;
}

void Update::saveActive(void) {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::ostringstream oss;
   oss << std::put_time(&tm, "%Y%m%d%H%M%S");
   std::string time_str = oss.str();
   json board_dict;
   board_dict["_id"] = time_str;
   std::ifstream data_file("/root/I2C/bin/active.json");
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
   Server* server = new Server();
   server->setDatabase("data");
   server->uploadDocument(board_dict);
   server->pushDatabase();
   delete server;
}

void Update::saveStatic(void) {
   Server* server = new Server();
   server->setDatabase("data");
   json doc = json::parse(server->getDocument("static"));
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
   std::ifstream data_file("/root/I2C/bin/active.json");
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
      sendFlag(board_dict, metadata, server);
   }
   server->uploadDocument(board_dict);
   server->pushDatabase();
   delete server;
}

void Update::sendFlag(json data, json metadata, Server* server) {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::ostringstream oss;
   oss << std::put_time(&tm, "%Y%m%d%H%M%S");
   std::string time_str = oss.str();
   json flag_dict = {
      {"_id",config["target"]["dbname"].get<std::string>() + "-" + time_str},
      {"properties", data},
      {"metadata", metadata}
   };
   server->uploadDocument(config["target"]["url"].get<std::string>()+"/flags",flag_dict);
}

void Update::purgeDatabase(void) {
   Server* server = new Server();
   server->setDatabase("data");
   if(server->checkOnline(config["target"]["url"].get<std::string>())) {
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);
      std::ostringstream oss;
      oss << std::put_time(&tm, "%Y%m%d%H%M%S");
      int time_int = atoi(oss.str().c_str());
      std::vector<std::pair<std::string,std::string> > IDs = server->getDocumentIDs();
      for (auto i : IDs) {
         int ID_int = atoi(i.first.c_str());
         if ((time_int - ID_int) > 10000) {
            server->deleteDocument(i.first, i.second);
         }
      }
      server->compactDatabase();
   }
   delete server;
}

void Update::writeConfig(void) {
   Server* server = new Server();
   server->setDatabase("config");
   json doc = json::parse(server->getDocument(config["target"]["dbname"]));
   delete server;
   for (json::iterator bus = doc.begin(); bus != doc.end(); ++bus) {
      if (bus.key().at(0) != '_') {
         board->setBus(bus.key());
         for (json::iterator device = bus.value().begin(); device != bus.value().end(); ++device) {
            board->setDevice(device.key());
            for (json::iterator property = device.value().begin(); property != device.value().end(); ++property) {
               board->write(property.key(), property.value());
            }
         }
      }
   }
}

void Update::getConfig(void) {
   std::string filename = config["config_db"]["path"].get<std::string>();
   int rev = (int)boost::filesystem::last_write_time(filename.c_str());
   if (rev != config["config_db"]["rev"].get<int>()) {
      std::cout << "Changing registers..." << '\n';
      writeConfig();
      config["config_db"]["rev"] = rev;
      std::ofstream config_file("/root/I2C/bin/config.json");
      config_file << std::setw(4) << config << std::endl;
   }
}
