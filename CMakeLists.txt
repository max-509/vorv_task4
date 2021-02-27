cmake_minimum_required(VERSION 3.10)
project(task4)

find_package(Boost REQUIRED COMPONENTS thread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

set(CMAKE_CXX_STANDARD 11)

add_executable(task4 main.cpp console_http/ConsoleHttpServer.cpp console_http/ConsoleHttpServer.h print_html_http/HelloWorldHttpServer.cpp print_html_http/HelloWorldHttpServer.h http_message/http_message_parser.cpp http_message/http_message_parser.h http_message/http_message_request.cpp http_message/http_message_request.h http_message/http_message_start_line.cpp http_message/http_message_start_line.h http_message/http_message_header.cpp http_message/http_message_header.h connection/connection.cpp connection/connection.h http_server/http_server.cpp http_server/http_server.h http_message/http_message_reply.cpp http_message/http_message_reply.h thread_pool_http/ThreadPoolHttpServer.cpp)