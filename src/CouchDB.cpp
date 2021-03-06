/**
  @file CouchDB.cpp
  @brief Defines functions for the base CouchDB class and its derived children.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "CouchDB.hpp"

/**
   @brief Callback function for curl HTTP calls. Sends output into string. Should not be called outside of CURLOPT_WRITEFUNCTION.
*/
size_t CouchDB::CallbackFunc(void *contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e) {
        return 0;
    }
    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

/**
   @brief HTTP GET function.
   @param url_ - URL to get from.
   @return String of output from call.
*/
bool CouchDB::HTTPGET(std::string url_, std::string* ret) {
   CURL* curl = curl_easy_init();
   CURLcode res;
   if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, ret);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
           fprintf(stderr, "curl_easy_perform() in function HTTPGET failed: %s\n", curl_easy_strerror(res));
           curl_easy_cleanup(curl);
           return false;
      }
   }
   curl_easy_cleanup(curl);
   return true;
}

/**
   @brief HTTP PUT function.
   @param url_ - URL to put data.
   @param data - Data to be put into file.
*/
bool CouchDB::HTTPPUT(std::string url_, std::string data) {
   CURL* curl = curl_easy_init();
   CURLcode res;
   std::string s;
   if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
           fprintf(stderr, "curl_easy_perform() in function HTTPPUT failed: %s\n", curl_easy_strerror(res));
           curl_easy_cleanup(curl);
           return false;
      }
   }
   curl_easy_cleanup(curl);
   return true;
}

/**
   @brief HTTP POST function.
   @param url_ - URL to post data.
   @param data - Data to be posted into file.
*/
bool CouchDB::HTTPPOST(std::string url_, std::string data) {
   CURL* curl = curl_easy_init();
   CURLcode res;
   std::string s;
   struct curl_slist *headers=NULL;
   if (curl) {
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_POST, true);
      curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
           fprintf(stderr, "curl_easy_perform() in function HTTPPOST failed: %s\n", curl_easy_strerror(res));
           std::cout << url_ << '\n';
           std::cout << data << '\n';
           curl_easy_cleanup(curl);
           return false;
      }
   }
   curl_easy_cleanup(curl);
   return true;
}

/**
   @brief HTTP DELETE function.
   @param url_ - URL of file to delete.
*/
bool CouchDB::HTTPDELETE(std::string url_) {
   CURL* curl = curl_easy_init();
   CURLcode res;
   std::string s;
   if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
           fprintf(stderr, "curl_easy_perform() in function HTTPDELETE failed: %s\n", curl_easy_strerror(res));
           curl_easy_cleanup(curl);
           return false;
      }
   }
   curl_easy_cleanup(curl);
   return true;
}

/**
   @brief Class constructor.
*/
Client::Client(void) {
   url = "http://127.0.0.1:5984";
   std::ifstream config_file("/root/I2C/bin/config.json");
   config_file >> config;
}

/**
   @brief Class constructor.
   @param url_ - URL of CouchDB server.
*/
Client::Client(std::string url_) {
   url = url_;
   std::ifstream config_file("/root/I2C/bin/config.json");
   config_file >> config;
}

/**
   @brief Class destructor.
*/
Client::~Client(void) {}

/**
   @brief Get list of databases on server.
   @return Vector of strings of IDs of each database.
*/
std::vector<std::string> Client::getDatabases(void) {
   std::vector<std::string> v;
   std::string s;
   HTTPGET(url+"/_all_dbs", &s);
   for (auto& element : json::parse(s)) {
      v.push_back(element);
   }
   return v;
}

/**
   @brief Set ID of database to work with.
   @param db - Database ID.
*/
void Client::setDatabase(std::string db) {
   database = db;
}

/**
   @brief Upload document to database.
   @param data - JSON table to upload.
*/
void Client::uploadDocument(json data) {
   if (!database.empty()) {
      std::string url_ = url + "/" + database + "/" + data["_id"].get<std::string>();
      HTTPPUT(url_, data.dump());
   }
}

/**
   @brief Upload document to database.
   @param url_ - URL of database to upload to.
   @param data - JSON table to upload.
*/
void Client::uploadDocument(std::string url_, json data) {
   std::string url__ = url_ + "/" + data["_id"].get<std::string>();
   HTTPPUT(url__, data.dump());
}

/**
   @brief Replicate database to master server.
*/
void Client::pushDatabase(void) {
   json data = {{"source",database},
                {"target",config["target"]["url"].get<std::string>() + "/" + config["target"]["dbname"].get<std::string>()},
                {"filter","filters/replicate_filter"}};
   HTTPPOST(url + "/_replicate", data.dump());
}

/**
   @brief Replicate database to master server with specific parameters.
   @param params - JSON key-value pairs for filter function.
*/
void Client::pushDatabase(json params) {
   json data = {{"source",database},
                {"target",config["target"]["url"].get<std::string>() + "/" + config["target"]["dbname"].get<std::string>()},
                {"filter","filters/replicate_filter"}};
   data["query_params"] = params;
   HTTPPOST(url + "/_replicate", data.dump());
}

/**
   @brief Upload document to database.
   @param doc - Document ID.
   @return String representation of JSON output.
*/
std::string Client::getDocument(std::string doc) {
   std::string url_ = url + "/"  + database + "/" + doc;
   std::string data;
   HTTPGET(url_, &data);
   return data;
}

/**
   @brief Get document IDs and revision IDs for given database.
   @return Vector of string pairs for ID and revision.
*/
std::vector<std::pair<std::string,std::string> > Client::getDocumentIDs(void) {
   std::vector<std::pair<std::string,std::string> > v;
   std::string s;
   HTTPGET(url+"/"+database+"/_design/"+database+"/_view/listAllActive", &s);
   for (auto& element : json::parse(s)["rows"]) {
      std::pair<std::string,std::string> p(element["key"].get<std::string>(), element["value"].get<std::string>());
      v.push_back(p);
   }
   return v;
}

/**
   @brief Check if server is online.
   @param url_ - URL of server to check.
   @return True if online, false if not.
*/
bool Client::checkOnline(std::string url_) {
   std::string ret;
   return HTTPGET(url_, &ret);
}

/**
   @brief Delete document from database.
   @param ID - ID of document.
   @param rev - revision of document.
*/
void Client::deleteDocument(std::string ID, std::string rev) {
   std::string url_ = url + "/" + database + "/" + ID + "?rev=" + rev;
   HTTPDELETE(url_);
}

/**
   Compact database to save space on local machine.
*/
void Client::compactDatabase(void) {
   std::string url_ = url + "/" + database + "/_compact";
   HTTPPOST(url_, "");
}

/**
   @brief Class constructor.
*/
Server::Server(void) {
   url = "http://127.0.0.1:5984";
   std::string ret;
   HTTPGET(url+"/slaves/_all_docs", &ret);
   slaves = json::parse(ret)["rows"];
}

/**
   @brief Class destructor.
*/
Server::~Server(void) {}

/**
   @brief Edit config entry for all slaves.
   @param bus - Bus of property.
   @param device - Device of property.
   @param property - Property ID.
   @param value - Value to be written.
*/
void Server::editConfig(std::string bus, std::string device, std::string property, std::string value) {
   for (auto database : slaves) {
      std::string ret;
      HTTPGET(url+"/config/"+database["id"].get<std::string>(), &ret);
      json doc = json::parse(ret);
      doc[bus][device][property] = value;
      std::string url_ = url + "/config/" + database["id"].get<std::string>();
      HTTPPUT(url_,doc.dump());
   }
}

/**
   @brief Edit config entry for specific board.
   @param bus - Bus of property.
   @param device - Device of property.
   @param property - Property ID.
   @param value - Value to be written.
*/
void Server::editConfig(std::string board, std::string bus, std::string device, std::string property, std::string value) {
   std::string ret;
   HTTPGET(url + "/config/" + board, &ret);
   json doc = json::parse(ret);
   doc[bus][device][property] = value;
   std::string url_ = url + "/config/" + board;
   HTTPPUT(url_, doc.dump());
}

/**
   @brief Push config database to slave.
   @param database - JSON of metadat for database to psush to.
*/
void Server::pushDatabase(json database) {
   std::string ret;
   HTTPGET(url + "/slaves/" + database["id"].get<std::string>(), &ret);
   json slave = json::parse(ret);
   json data = {{"source","config"},
                {"target", slave["address"].get<std::string>() + "/config"},
                {"doc_ids",{database["id"].get<std::string>()}}};
   HTTPPOST(url+"/_replicate", data.dump());
}

/**
   @brief Push config databases to slaves.
*/
void Server::pushChanges(void) {
   for (auto database : slaves) {
      pushDatabase(database);
   }
}
