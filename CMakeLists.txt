cmake_minimum_required(VERSION 3.16)
project(HttpWebServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

add_executable(http_server
    src/main.cpp
    src/http/server.cpp
    src/http/request.cpp
    src/http/response.cpp
    src/router/router.cpp
    src/session/session_manager.cpp
    src/session/token.cpp
    src/utils/thread_pool.cpp
)

target_include_directories(http_server PRIVATE src)
target_link_libraries(http_server Threads::Threads)
