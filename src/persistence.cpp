
#include "persistence.h"
#include "store.h"
#include <fstream>
AOF::AOF(const std::string& path): path_(path) {}
void AOF::append(const std::string& line) {
    std::lock_guard<std::mutex> lg(mu_);
    std::ofstream f(path_, std::ios::app);
    f<<line<<"\n";
}
void AOF::replay(Store& store) {
    std::ifstream f(path_);
    if (!f) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.rfind("SET ",0)==0) {
            auto rest = line.substr(4);
            auto sp = rest.find(' ');
            if (sp!=std::string::npos) {
                auto key = rest.substr(0,sp);
                auto val = rest.substr(sp+1);
                store.set(key, val);
            }
        } else if (line.rfind("DEL ",0)==0) {
            auto key = line.substr(4);
            bool ex=false; store.del(key, ex);
        }
    }
}
