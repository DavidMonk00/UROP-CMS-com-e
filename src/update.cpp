/**
  @file update.cpp
  @brief Defines functions for the Update class.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "update.hpp"

/**
   @brief Class constructor.
*/
Update::Update(void) {
   board = new ATCABoard("SEMA");
   std::ifstream config_file("/root/I2C/bin/config.json");
   config_file >> config;
}

/**
   @brief Class destructor.
*/
Update::~Update(void) {
   delete board;
}

/**
   @brief Save active registers to local cache and then push the database to an external server.
*/
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
   Client* client = new Client();
   client->setDatabase("data");
   client->uploadDocument(board_dict);
   client->pushDatabase();
   delete client;
}

/**
   @brief Save static registers to local cache and then push the database to an external server.
*/
void Update::saveStatic(void) {
   Client* client = new Client();
   client->setDatabase("data");
   json doc = json::parse(client->getDocument("static"));
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
      sendFlag(board_dict, metadata, client);
   }
   client->uploadDocument(board_dict);
   client->pushDatabase();
   delete client;
}

/**
   @brief Send flag to external server if static register is changed.
   @param data - register data dump at time of change.
   @param metadata - JSON of metadata for flag.
   @param client - CouchDB Client class to send data to server.
*/
void Update::sendFlag(json data, json metadata, Client* client) {
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
   client->uploadDocument(config["target"]["url"].get<std::string>()+"/flags",flag_dict);
}

/**
   @brief Purge database of any entries over an hour old.
*/
void Update::purgeDatabase(void) {
   Client* client = new Client();
   client->setDatabase("data");
   if(client->checkOnline(config["target"]["url"].get<std::string>())) {
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);
      std::ostringstream oss;
      oss << std::put_time(&tm, "%Y%m%d%H%M%S");
      int time_int = atoi(oss.str().c_str());
      std::vector<std::pair<std::string,std::string> > IDs = client->getDocumentIDs();
      for (auto i : IDs) {
         int ID_int = atoi(i.first.c_str());
         if ((time_int - ID_int) > 10000) {
            client->deleteDocument(i.first, i.second);
         }
      }
      client->compactDatabase();
   }
   delete client;
}

/**
   @brief Write config database to registers on card.
*/
void Update::writeConfig(void) {
   Client* client = new Client();
   client->setDatabase("config");
   json doc = json::parse(client->getDocument(config["target"]["dbname"]));
   delete client;
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

/**
   @brief Check if config files have changed and update registers if true.
*/
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
