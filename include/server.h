
#pragma once
#include <string>
#include <atomic>
struct Config {
    std::string mode;
    std::string host;
    int port;
    std::string aof_path;
    int max_clients;
};
bool load_config(const std::string& path, Config& out);
class Server {
public:
    Server(const Config& cfg);
    ~Server();
    void run();
private:
    Config cfg_;
    int listen_fd_ = -1;
    std::atomic<bool> running_{true};
    void accept_loop();
    void client_loop(int client_fd, std::string peer);
    void handle_command(int client_fd, const std::string& line, std::string& out);
};
