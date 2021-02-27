#include <iostream>

#include <memory>
#include <boost/asio.hpp>

#include "console_http/ConsoleHttpServer.h"
#include "print_html_http/HelloWorldHttpServer.h"
#include "thread_pool_http/ThreadPoolHttpServer.h"

int main() {
    unsigned short port = 8080;
//    ConsoleHttpServer console_http_server(port);
//    console_http_server.start();

//    HelloWorldHttpServer hello_world_http_server(port);
//    hello_world_http_server.start();

    ThreadPoolHttpServer thread_pool_http_server{port};
    thread_pool_http_server.start();

    return 0;
}
