#pragma once
#include <string>

namespace http {

class Token {
public:
    static std::string generate(const std::string& user_id);
    static bool validate(const std::string& token, std::string& user_id_out);

private:
    static std::string sign(const std::string& payload);
    static std::string hmac(const std::string& data, const std::string& key);
};

} // namespace http
