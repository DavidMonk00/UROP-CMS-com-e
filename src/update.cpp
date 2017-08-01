#include "update.h"

Update::Update(void) {
   // board = new ATCABoard("SEMA");
   // server = new Server();
}

Update::~Update(void) {
   // delete board;
   // delete server;
}

void Update::saveActive(void) {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::ostringstream oss;
   oss << std::put_time(&tm, "%Y%m%d%H%M%S");
   std::string time_str = oss.str();
   std::string id_str("_id");
   json board_dict;
   board_dict.emplace(id_str,time_str);
   std::ifstream data_file("bin/active.json");
   json active;
   data_file >> active;
   for (auto bus : board->getBuses()) {
      json bus_dict;
      board->setBus(bus);
      for (auto device : board->getDevices()) {
         json device_dict;
         board->setDevice(device);
         for (auto property : board->getProperties()) {
            if (active[bus][device][property]) {
               device_dict.emplace(property,board->read(property));
            }
         }
         bus_dict.emplace(device, device_dict);
      }
      board_dict.emplace(bus, bus_dict);
   }
   std::cout << board_dict << '\n';
}
