#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

class Server {
private:
   std::string url;
   std::string database;
   CURL* curl;
   static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string* s);
   std::string HTTPGET(std::string hostname);
public:
   Server(void);
   Server(std::string url_);
   ~Server(void);
   std::vector<std::string> getDatabases(void);
   void setDatabase(std::string db);
};
