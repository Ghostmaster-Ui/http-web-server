#pragma once
#include <string>
#include <unordered_map>

namespace http {

enum class Method { GET, POST, PUT, DELETE, UNKNOWN };

struct Request {
    Method method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::unordered_map<std::string, std::string> params;

    static Request parse(const std::string& raw);
    static Method parse_method(const std::string& m);
    std::string get_header(const std::string& key) const;
};

} // namespace http
