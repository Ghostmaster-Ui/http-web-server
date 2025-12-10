#pragma once
#include <atomic>
#include "router/router.h"
#include "session/session_manager.h"
#include "utils/thread_pool.h"

namespace http {

class Server {
public:
    Server(int port, size_t num_threads = 8);
    ~Server();

    void get(const std::string& path, Handler handler);
    void post(const std::string& path, Handler handler);
    void start();
    void stop();

    SessionManager& sessions() { return sessions_; }

private:
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    Router router_;
    SessionManager sessions_;
    ThreadPool pool_;

    void accept_loop();
    void handle_client(int client_fd);
};

} // namespace http
