#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>

namespace http {

Server::Server(int port, size_t num_threads)
    : port_(port), server_fd_(-1), running_(false), pool_(num_threads) {}

Server::~Server() { stop(); }

void Server::get(const std::string& path, Handler handler) {
    router_.get(path, handler);
}

void Server::post(const std::string& path, Handler handler) {
    router_.post(path, handler);
}

void Server::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0)
        throw std::runtime_error("Failed to create socket");

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind");

    if (listen(server_fd_, 256) < 0)
        throw std::runtime_error("Failed to listen");

    running_ = true;
    std::cout << "HTTP Server listening on port " << port_ << std::endl;
    accept_loop();
}

void Server::stop() {
    running_ = false;
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
}

void Server::accept_loop() {
    while (running_) {
        int client_fd = accept(server_fd_, nullptr, nullptr);
        if (client_fd < 0) continue;
        pool_.enqueue([this, client_fd]() {
            handle_client(client_fd);
        });
    }
}

void Server::handle_client(int client_fd) {
    char buf[8192] = {};
    int n = read(client_fd, buf, sizeof(buf) - 1);
    if (n <= 0) { close(client_fd); return; }

    try {
        Request req = Request::parse(std::string(buf, n));
        Response res = router_.route(req);
        std::string raw = res.to_string();
        write(client_fd, raw.c_str(), raw.size());
    } catch (...) {
        Response res = Response::bad_request();
        std::string raw = res.to_string();
        write(client_fd, raw.c_str(), raw.size());
    }
    close(client_fd);
}

} // namespace http
