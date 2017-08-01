#include "couchdb.h"

Server::Server(void) {
   url = "http://127.0.0.1:5984";
   std::ifstream file("bin/config.txt");
   std::string str;
   while (file >> str) {
      target.push_back(str);
   }
   curl_global_init(CURL_GLOBAL_DEFAULT);
}

Server::Server(std::string url_) {
   url = url_;
   std::ifstream file("bin/config.txt");
   std::string str;
   while (file >> str) {
      target.push_back(str);
   }
   curl_global_init(CURL_GLOBAL_DEFAULT);
}

Server::~Server(void) {
   curl_easy_cleanup(curl);
}

size_t Server::CallbackFunc(void *contents, size_t size, size_t nmemb, std::string* s) {
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

std::string Server::HTTPGET(std::string url_) {
   curl = curl_easy_init();
   CURLcode res;
   std::string s;
   if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
           fprintf(stderr, "curl_easy_perform() failed: %s\n",
                   curl_easy_strerror(res));
      }
   }
   return s;
}

void Server::HTTPPUT(std::string url_, std::string data) {
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
           fprintf(stderr, "curl_easy_perform() failed: %s\n",
                   curl_easy_strerror(res));
      }
   }
}

void Server::HTTPPOST(std::string url_, std::string data) {
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
           fprintf(stderr, "curl_easy_perform() failed: %s\n",
                   curl_easy_strerror(res));
      }
   }
}

std::vector<std::string> Server::getDatabases(void) {
   std::vector<std::string> v;
   std::string s = HTTPGET("http://127.0.0.1:5984/_all_dbs");
   auto j = json::parse(s);
   for (auto& element : j) {
      v.push_back(element);
   }
   return v;
}

void Server::setDatabase(std::string db) {
   database = db;
}

void Server::uploadDocument(json data) {
   if (!database.empty()) {
      std::string doc_id = data["_id"];
      std::string url_ = url + "/" + database + "/" + doc_id;
      std::string jsn = data.dump();
      HTTPPUT(url_,jsn);
   }
}

void Server::uploadDocument(std::string url_, json data) {
   std::string doc_id = data["_id"];
   std::string url__ = url_ + "/" + doc_id;
   std::string jsn = data.dump();
   HTTPPUT(url__,jsn);
}

void Server::pushDatabase(void) {
   json data = {{"source","data"},
                {"target",target[0] + target[1]},
                {"filter","filters/replicate_filter"}};
   HTTPPOST("http://127.0.0.1:5984/_replicate", data.dump());
}

json Server::getStaticDocument(void) {
   std::string url_ = url + "/"  + database + "/static";
   std::string data = HTTPGET(url_);
   return json::parse(data);
}
