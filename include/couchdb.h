#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include "json.hpp"

using json = nlohmann::json;

class Server {
private:
   std::string url;
   std::string database;
   json config;
   CURL* curl;
   static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string* s);
   std::string HTTPGET(std::string hostname);
   void HTTPPUT(std::string url_, std::string data);
   void HTTPPOST(std::string url_, std::string data);
   void HTTPDELETE(std::string url_);
public:
   Server(void);
   Server(std::string url_);
   ~Server(void);
   std::vector<std::string> getDatabases(void);
   void setDatabase(std::string db);
   void uploadDocument(json data);
   void uploadDocument(std::string url_, json data);
   void pushDatabase(void);
   void pushDatabase(json params);
   std::string getDocument(std::string doc);
   std::vector<std::pair<std::string,std::string> > getDocumentIDs(void);
   bool checkOnline(std::string url_);
   void deleteDocument(std::string ID, std::string rev);
   void compactDatabase(void);
};
