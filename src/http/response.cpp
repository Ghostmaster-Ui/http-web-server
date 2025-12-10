#include "response.h"
#include <sstream>

namespace http {

Response::Response(int code, const std::string& text)
    : status_code(code), status_text(text) {
    headers["Server"] = "AdvitHTTP/1.0";
    headers["Connection"] = "close";
}

void Response::set_header(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void Response::set_body(const std::string& content, const std::string& content_type) {
    body = content;
    headers["Content-Type"] = content_type;
    headers["Content-Length"] = std::to_string(content.size());
}

std::string Response::to_string() const {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status_code << " " << status_text << "\r\n";
    for (const auto& [k, v] : headers)
        oss << k << ": " << v << "\r\n";
    oss << "\r\n" << body;
    return oss.str();
}

Response Response::ok(const std::string& body, const std::string& type) {
    Response r(200, "OK");
    r.set_body(body, type);
    return r;
}

Response Response::not_found() {
    Response r(404, "Not Found");
    r.set_body("404 Not Found");
    return r;
}

Response Response::bad_request() {
    Response r(400, "Bad Request");
    r.set_body("400 Bad Request");
    return r;
}

Response Response::unauthorized() {
    Response r(401, "Unauthorized");
    r.set_body("401 Unauthorized");
    return r;
}

} // namespace http
