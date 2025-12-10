#include "router.h"

namespace http {

void Router::add(Method method, const std::string& path, Handler handler) {
    routes_.push_back({method, path, handler});
}

void Router::get(const std::string& path, Handler handler) {
    add(Method::GET, path, handler);
}

void Router::post(const std::string& path, Handler handler) {
    add(Method::POST, path, handler);
}

void Router::put(const std::string& path, Handler handler) {
    add(Method::PUT, path, handler);
}

void Router::del(const std::string& path, Handler handler) {
    add(Method::DELETE, path, handler);
}

Response Router::route(const Request& req) const {
    for (const auto& route : routes_) {
        if (route.method == req.method && route.path == req.path)
            return route.handler(req);
    }
    return Response::not_found();
}

} // namespace http
