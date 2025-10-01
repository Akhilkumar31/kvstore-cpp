
#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
class Store {
public:
    std::string get(const std::string& key, bool& found);
    std::string set(const std::string& key, const std::string& value);
    std::string del(const std::string& key, bool& existed);
private:
    std::unordered_map<std::string, std::string> kv_;
    std::mutex mu_;
};
