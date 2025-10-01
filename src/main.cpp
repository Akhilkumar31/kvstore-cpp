
#include "server.h"
#include <iostream>
#include <cstdlib>
int main(){
    const char* cfgp = std::getenv("CONFIG_FILE");
    std::string cfg = cfgp ? cfgp : "config.yaml";
    Config c;
    if (!load_config(cfg, c)) { std::cerr<<"Failed to load "<<cfg<<"\n"; return 1; }
    Server s(c);
    s.run();
    return 0;
}
