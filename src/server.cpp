
#include "server.h"
#include "store.h"
#include "persistence.h"
#include "util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include<thread>
static Store g_store;
static AOF* g_aof = nullptr;

static int create_listener(const std::string& host, int port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0) return -1;
    if (listen(fd, 64) < 0) return -1;
    return fd;
}

bool load_config(const std::string& path, Config& out) {
    std::ifstream f(path);
    if (!f) return false;
    std::string line;
    while (std::getline(f, line)) {
        auto pos = line.find(':');
        if (pos == std::string::npos) continue;
        auto k = trim(line.substr(0,pos));
        auto v = trim(line.substr(pos+1));
        if (!v.empty() && v.front()=='"') v = v.substr(1, v.size()-2);
        if (k=="mode") out.mode = v;
        else if (k=="host") out.host = v;
        else if (k=="port") out.port = std::stoi(v);
        else if (k=="aof_path") out.aof_path = v;
        else if (k=="max_clients") out.max_clients = std::stoi(v);
    }
    return true;
}

Server::Server(const Config& cfg): cfg_(cfg) {}
Server::~Server(){ if(listen_fd_>=0) close(listen_fd_); }

void Server::run() {
    system("mkdir -p data");
    g_aof = new AOF(cfg_.aof_path);
    g_aof->replay(g_store);
    listen_fd_ = create_listener(cfg_.host, cfg_.port);
    if (listen_fd_<0) { std::cerr<<"listen failed\n"; return; }
    std::cout<<"KV listening on "<<cfg_.host<<":"<<cfg_.port<<"\n";
    accept_loop();
}

void Server::accept_loop() {
    while (running_) {
        sockaddr_in cli{}; socklen_t cl = sizeof(cli);
        int cfd = accept(listen_fd_, (sockaddr*)&cli, &cl);
        if (cfd<0) continue;
        std::string peer = inet_ntoa(cli.sin_addr);
        std::thread(&Server::client_loop, this, cfd, peer).detach();
    }
}

void Server::client_loop(int client_fd, std::string peer) {
    char buf[4096]; std::string acc;
    while (true) {
        ssize_t n = recv(client_fd, buf, sizeof(buf), 0);
        if (n<=0) break;
        acc.append(buf, buf+n);
        size_t pos;
        while ((pos = acc.find('\n')) != std::string::npos) {
            std::string line = trim(acc.substr(0,pos));
            acc.erase(0,pos+1);
            std::string out;
            handle_command(client_fd, line, out);
            out.push_back('\n');
            send(client_fd, out.c_str(), out.size(), 0);
        }
    }
    close(client_fd);
}

void Server::handle_command(int client_fd, const std::string& line, std::string& out) {
    auto toks = split_ws(line);
    if (toks.empty()) { out="ERR empty"; return; }
    auto cmd = toks[0];
    if (cmd=="PING") { out="PONG"; return; }
    if (cmd=="GET" && toks.size()==2) {
        bool found=false; auto v = g_store.get(toks[1], found);
        out = found? ("$ "+v) : "(nil)"; return;
    }
    if (cmd=="SET" && toks.size()>=3) {
        std::string key = toks[1];
        std::string value = line.substr(line.find(key)+key.size()+1);
        auto r = g_store.set(key, value);
        g_aof->append("SET "+key+" "+value);
        out = r; return;
    }
    if (cmd=="DEL" && toks.size()==2) {
        bool ex=false; auto r = g_store.del(toks[1], ex);
        if (ex) g_aof->append("DEL "+toks[1]);
        out = r; return;
    }
    out="ERR unknown/arity";
}
