
#include "util.h"
#include <sstream>
#include <cctype>
std::vector<std::string> split_ws(const std::string& s) {
    std::istringstream iss(s); std::vector<std::string> out; std::string tok;
    while (iss>>tok) out.push_back(tok); return out;
}
std::string trim(const std::string& s) {
    size_t i=0, j=s.size();
    while (i<j && std::isspace((unsigned char)s[i])) ++i;
    while (j>i && std::isspace((unsigned char)s[j-1])) --j;
    return s.substr(i, j-i);
}
bool starts_with(const std::string& s, const std::string& p) { return s.rfind(p,0)==0; }
