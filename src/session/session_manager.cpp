#include "session_manager.h"
#include "token.h"

namespace http {

std::string SessionManager::create(const std::string& user_id) {
    std::string token = Token::generate(user_id);
    std::unique_lock lock(mutex_);
    sessions_[token] = {user_id, token};
    return token;
}

std::optional<Session> SessionManager::get(const std::string& token) {
    std::string user_id;
    if (!Token::validate(token, user_id)) return std::nullopt;
    std::shared_lock lock(mutex_);
    auto it = sessions_.find(token);
    if (it == sessions_.end()) return std::nullopt;
    return it->second;
}

bool SessionManager::invalidate(const std::string& token) {
    std::unique_lock lock(mutex_);
    return sessions_.erase(token) > 0;
}

size_t SessionManager::size() const {
    std::shared_lock lock(mutex_);
    return sessions_.size();
}

} // namespace http
