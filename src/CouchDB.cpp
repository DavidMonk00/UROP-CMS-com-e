#include "CouchDB.hpp"

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

std::string CouchDB::HTTPGET(std::string url_) {
   curl = curl_easy_init();
   CURLcode res;
   std::string s;
   if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
           fprintf(stderr, "curl_easy_perform() in function HTTPGET failed: %s\n", curl_easy_strerror(res));
      }
   }
   return s;
}

void CouchDB::HTTPPUT(std::string url_, std::string data) {
   curl = curl_easy_init();
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
      }
   }
}

void CouchDB::HTTPPOST(std::string url_, std::string data) {
   curl = curl_easy_init();
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
      }
   }
}

void CouchDB::HTTPDELETE(std::string url_) {
   curl = curl_easy_init();
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
      }
   }
}

Client::Client(void) {
   url = "http://127.0.0.1:5984";
   std::ifstream config_file("/root/I2C/bin/config.json");
   config_file >> config;
}

Client::Client(std::string url_) {
   url = url_;
   std::ifstream config_file("/root/I2C/bin/config.json");
   config_file >> config;
}

Client::~Client(void) {
   curl_easy_cleanup(curl);
}

std::vector<std::string> Client::getDatabases(void) {
   std::vector<std::string> v;
   std::string s = HTTPGET(url+"/_all_dbs");
   auto j = json::parse(s);
   for (auto& element : j) {
      v.push_back(element);
   }
   return v;
}

void Client::setDatabase(std::string db) {
   database = db;
}

void Client::uploadDocument(json data) {
   if (!database.empty()) {
      std::string doc_id = data["_id"];
      std::string url_ = url + "/" + database + "/" + doc_id;
      std::string jsn = data.dump();
      HTTPPUT(url_,jsn);
   }
}

void Client::uploadDocument(std::string url_, json data) {
   std::string doc_id = data["_id"];
   std::string url__ = url_ + "/" + doc_id;
   std::string jsn = data.dump();
   HTTPPUT(url__,jsn);
}

void Client::pushDatabase(void) {
   json data = {{"source",database},
                {"target",config["target"]["url"].get<std::string>() + "/" + config["target"]["dbname"].get<std::string>()},
                {"filter","filters/replicate_filter"}};
   HTTPPOST(url+"/_replicate", data.dump());
}

void Client::pushDatabase(json params) {
   json data = {{"source",database},
                {"target",config["target"]["url"].get<std::string>() + "/" + config["target"]["dbname"].get<std::string>()},
                {"filter","filters/replicate_filter"}};
   data["query_params"] = params;
   HTTPPOST(url+"/_replicate", data.dump());
}

std::string Client::getDocument(std::string doc) {
   std::string url_ = url + "/"  + database + "/" + doc;
   std::string data = HTTPGET(url_);
   return data;
}

std::vector<std::pair<std::string,std::string> > Client::getDocumentIDs(void) {
   std::vector<std::pair<std::string,std::string> > v;
   std::string s = HTTPGET(url+"/"+database+"/_design/"+database+"/_view/listAllActive");
   auto j = json::parse(s);
   for (auto& element : j["rows"]) {
      std::pair<std::string,std::string> p(element["key"].get<std::string>(), element["value"].get<std::string>());
      v.push_back(p);
   }
   return v;
}

bool Client::checkOnline(std::string url_) {
   std::string ret = HTTPGET(url_);
   json j = json::parse(ret);
   return true;
}

void Client::deleteDocument(std::string ID, std::string rev) {
   std::string url_ = url + "/" + database + "/" + ID + "?rev=" + rev;
   HTTPDELETE(url_);
}

void Client::compactDatabase(void) {
   std::string url_ = url + "/" + database + "/_compact";
   HTTPPOST(url_, "");
}

Server::Server(void) {
   url = "http://127.0.0.1:5984";
   slaves = json::parse(HTTPGET(url+"/slaves/_all_docs"))["rows"];
}

Server::~Server(void) {
   curl_easy_cleanup(curl);
}

void Server::editConfig(std::string property, std::string value) {

}

void Server::editConfig(std::string device, std::string property, std::string value) {

}

void pushChanges(void) {
   for (auto i : slaves) {
      std::cout << i["_id"] << '\n';
   }
}
