#pragma once
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <optional>

namespace http {

struct Session {
    std::string user_id;
    std::string token;
};

class SessionManager {
public:
    std::string create(const std::string& user_id);
    std::optional<Session> get(const std::string& token);
    bool invalidate(const std::string& token);
    size_t size() const;

private:
    std::unordered_map<std::string, Session> sessions_;
    mutable std::shared_mutex mutex_;
};

} // namespace http
