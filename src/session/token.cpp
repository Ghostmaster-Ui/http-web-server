#include "token.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <functional>

namespace http {

static const std::string SECRET = "advit-secret-key-2025";

static std::string to_hex(uint64_t val) {
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << val;
    return oss.str();
}

std::string Token::hmac(const std::string& data, const std::string& key) {
    uint64_t h = 14695981039346656037ULL;
    for (char c : key) {
        h ^= static_cast<uint64_t>(c);
        h *= 1099511628211ULL;
    }
    for (char c : data) {
        h ^= static_cast<uint64_t>(c);
        h *= 1099511628211ULL;
    }
    return to_hex(h);
}

std::string Token::generate(const std::string& user_id) {
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    std::string payload = user_id + ":" + std::to_string(now);
    std::string sig = hmac(payload, SECRET);
    return payload + "." + sig;
}

bool Token::validate(const std::string& token, std::string& user_id_out) {
    auto dot = token.rfind('.');
    if (dot == std::string::npos) return false;
    std::string payload = token.substr(0, dot);
    std::string sig = token.substr(dot + 1);
    if (hmac(payload, SECRET) != sig) return false;
    auto colon = payload.find(':');
    if (colon == std::string::npos) return false;
    user_id_out = payload.substr(0, colon);
    return true;
}

} // namespace http
