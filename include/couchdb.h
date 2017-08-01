#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class Server {
private:
   std::string url;
   std::string database;
   std::vector<std::string> target;
   CURL* curl;
   static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string* s);
   std::string HTTPGET(std::string hostname);
   void HTTPPUT(std::string url_, std::string data);
   void HTTPPOST(std::string url_, std::string data);
public:
   Server(void);
   Server(std::string url_);
   ~Server(void);
   std::vector<std::string> getDatabases(void);
   void setDatabase(std::string db);
   void uploadDocument(json data);
   void uploadDocument(std::string url_, json data);
   void pushDatabase(void);
   json getStaticDocument(void);
};
