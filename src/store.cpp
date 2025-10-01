
#include "store.h"
std::string Store::get(const std::string& key, bool& found) {
    std::lock_guard<std::mutex> lg(mu_);
    auto it = kv_.find(key);
    if (it==kv_.end()) { found=false; return ""; }
    found=true; return it->second;
}
std::string Store::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lg(mu_);
    kv_[key] = value; return "OK";
}
std::string Store::del(const std::string& key, bool& existed) {
    std::lock_guard<std::mutex> lg(mu_);
    auto it = kv_.find(key);
    if (it==kv_.end()) { existed=false; return "(nil)"; }
    kv_.erase(it); existed=true; return "OK";
}
