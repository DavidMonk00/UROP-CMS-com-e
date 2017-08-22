/**
  main.cpp
  Purpose: main function file.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "main.hpp"

void activeLoop(Update* update) {
   while (true) {
      std::thread t_sleep(sleep, SLEEP_TIME);
      update->saveActive();
      t_sleep.join();
   }
}

void configLoop(Update* update) {
   while (true) {
      std::thread t_sleep(sleep, SLEEP_TIME);
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
         update->purgeDatabase();
         sleep(65);
      } else {
         sleep(30);
      }
   }
}

int main(int argc, char* argv[]) {
   curl_global_init(CURL_GLOBAL_DEFAULT);
   Update* update = new Update(new I2CSema(EAPI_ID_I2C_EXTERNAL));
   std::thread t_active(activeLoop, update);
   std::thread t_config(configLoop, update);
   std::thread t_static(staticLoop, update);
   std::thread t_purge(purgeLoop, update);
   t_active.join();
   t_config.join();
   t_static.join();
   t_purge.join();
   delete update;
}
