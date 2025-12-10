#pragma once
#include <string>
#include <unordered_map>

namespace http {

class Response {
public:
    int status_code;
    std::string status_text;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Response(int code = 200, const std::string& text = "OK");

    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& content, const std::string& content_type = "text/plain");
    std::string to_string() const;

    static Response ok(const std::string& body, const std::string& type = "text/plain");
    static Response not_found();
    static Response bad_request();
    static Response unauthorized();
};

} // namespace http
