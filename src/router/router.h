#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include "http/request.h"
#include "http/response.h"

namespace http {

using Handler = std::function<Response(const Request&)>;

struct Route {
    Method method;
    std::string path;
    Handler handler;
};

class Router {
public:
    void get(const std::string& path, Handler handler);
    void post(const std::string& path, Handler handler);
    void put(const std::string& path, Handler handler);
    void del(const std::string& path, Handler handler);

    Response route(const Request& req) const;

private:
    std::vector<Route> routes_;
    void add(Method method, const std::string& path, Handler handler);
};

} // namespace http
