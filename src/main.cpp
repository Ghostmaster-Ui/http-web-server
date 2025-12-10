#include <iostream>
#include "http/server.h"
#include "session/session_manager.h"
#include "session/token.h"

int main() {
    // Test token generation
    std::cout << "--- Token System ---" << std::endl;
    std::string token = http::Token::generate("advit");
    std::cout << "Generated token: " << token.substr(0, 40) << "..." << std::endl;

    std::string user_id;
    bool valid = http::Token::validate(token, user_id);
    std::cout << "Token valid: " << (valid ? "yes" : "no") << std::endl;
    std::cout << "User ID: " << user_id << std::endl;

    // Test session manager
    std::cout << "\n--- Session Manager ---" << std::endl;
    http::SessionManager sessions;
    std::string sess_token = sessions.create("advit");
    auto session = sessions.get(sess_token);
    if (session) std::cout << "Session user: " << session->user_id << std::endl;
    sessions.invalidate(sess_token);
    std::cout << "After invalidate: " << (sessions.get(sess_token) ? "found" : "NOT FOUND") << std::endl;

    // Start HTTP server
    std::cout << "\n--- Starting HTTP Server ---" << std::endl;
    http::Server server(8080, 8);

    server.get("/", [](const http::Request&) {
        return http::Response::ok("Welcome to AdvitHTTP/1.0", "text/plain");
    });

    server.get("/health", [](const http::Request&) {
        return http::Response::ok("{\"status\":\"ok\"}", "application/json");
    });

    server.post("/login", [&](const http::Request& req) {
        std::string token = server.sessions().create("advit");
        return http::Response::ok("{\"token\":\"" + token.substr(0,20) + "...\"}", "application/json");
    });

    server.get("/protected", [&](const http::Request& req) {
        std::string auth = req.get_header("authorization");
        if (auth.empty()) return http::Response::unauthorized();
        auto sess = server.sessions().get(auth);
        if (!sess) return http::Response::unauthorized();
        return http::Response::ok("Hello " + sess->user_id, "text/plain");
    });

    server.start();
    return 0;
}
