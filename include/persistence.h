
#pragma once
#include <string>
#include <mutex>
class Store;
class AOF {
public:
    explicit AOF(const std::string& path);
    void append(const std::string& line);
    void replay(Store& store);
private:
    std::string path_;
    std::mutex mu_;
};
