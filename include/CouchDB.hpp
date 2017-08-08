/**
  @file CouchDB.hpp
  @brief Defines the CouchDB base class and Server and Client derived classes.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include "json.hpp"

using json = nlohmann::json;

class CouchDB {
protected:
   std::string url;
   std::string database;
   json config;
   CURL* curl;
   static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string* s);
   std::string HTTPGET(std::string hostname);
   void HTTPPUT(std::string url_, std::string data);
   void HTTPPOST(std::string url_, std::string data);
   void HTTPDELETE(std::string url_);
};

class Client : public CouchDB {
public:
   Client(void);
   Client(std::string url_);
   ~Client(void);
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

class Server : public CouchDB {
private:
   json slaves;
public:
   Server(void);
   ~Server(void);
   void editConfig(std::string bus, std::string device, std::string property, std::string value);
   void editConfig(std::string board, std::string bus, std::string device, std::string property, std::string value);
   void pushDatabase(json database);
   void pushChanges(void);
};
