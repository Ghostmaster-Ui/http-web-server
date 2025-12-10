#include "request.h"
#include <sstream>
#include <algorithm>

namespace http {

Method Request::parse_method(const std::string& m) {
    if (m == "GET")    return Method::GET;
    if (m == "POST")   return Method::POST;
    if (m == "PUT")    return Method::PUT;
    if (m == "DELETE") return Method::DELETE;
    return Method::UNKNOWN;
}

Request Request::parse(const std::string& raw) {
    Request req;
    std::istringstream stream(raw);
    std::string line;

    std::getline(stream, line);
    if (!line.empty() && line.back() == '\r') line.pop_back();
    std::istringstream first_line(line);
    std::string method, path, version;
    first_line >> method >> path >> version;
    req.method = parse_method(method);
    req.path = path;
    req.version = version;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break;
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string val = line.substr(colon + 2);
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            req.headers[key] = val;
        }
    }

    std::string body_line;
    while (std::getline(stream, body_line))
        req.body += body_line + "\n";

    return req;
}

std::string Request::get_header(const std::string& key) const {
    auto it = headers.find(key);
    return it != headers.end() ? it->second : "";
}

} // namespace http
