
#include "util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
int main(int argc, char** argv){
    if (argc<3){ std::cerr<<"usage: kvclient <host> <port>\n"; return 1; }
    std::string host = argv[1]; int port = std::stoi(argv[2]);
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(port); addr.sin_addr.s_addr=inet_addr(host.c_str());
    if (connect(fd, (sockaddr*)&addr, sizeof(addr))<0){ std::cerr<<"connect failed\n"; return 1; }
    std::cout<<"Connected. Type commands like: PING | SET key value | GET key | DEL key\n";
    std::string line; char buf[4096];
    while (std::getline(std::cin, line)){
        line.push_back('\n');
        if (send(fd, line.c_str(), line.size(), 0)<=0) break;
        int n = recv(fd, buf, sizeof(buf)-1, 0);
        if (n<=0) break; buf[n]=0; std::cout<<buf;
    }
    close(fd); return 0;
}
