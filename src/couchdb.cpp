#include "couchdb.h"

Server::Server(void) {
   url = "127.0.0.1";
   curl_global_init(CURL_GLOBAL_DEFAULT);
   curl = curl_easy_init();
}

Server::Server(std::string url_) {
   url = url_;
   curl_global_init(CURL_GLOBAL_DEFAULT);
   curl = curl_easy_init();
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

std::string Server::HTTPGET(std::string url_) {
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
