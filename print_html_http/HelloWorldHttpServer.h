#ifndef TASK4_HELLOWORLDHTTPSERVER_H
#define TASK4_HELLOWORLDHTTPSERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <cstddef>
#include <iostream>
#include <array>

#include "../http_message/http_message_parser.h"
#include "../http_message/http_message_request.h"
#include "../http_server/http_server.h"
#include "../connection/connection.h"

using namespace boost::asio;

class HelloWorldHttpServer : public http_server {
private:
    std::string HELLO_WORLD_PAGE = "<!DOCTYPE html>\n"
                                   "<html lang=\"en\">\n"
                                   "\n"
                                   "    <head>\n"
                                   "        <meta charset=\"UTF-8\">\n"
                                   "        <title>Hello!</title>\n"
                                   "    </head>\n"
                                   "\n"
                                   "    <body>\n"
                                   "        <h1>Hello World!</h1>\n"
                                   "        <p>This is a simple paragraph.</p>\n"
                                   "    </body>\n"
                                   "\n"
                                   "</html>";
public:

    explicit HelloWorldHttpServer(unsigned short port);

    void start() override;

    void stop(std::shared_ptr<connection> con_ptr) override;

    void stop() override;

    void after_read_handler(std::shared_ptr<connection> con_ptr) override;
};


#endif //TASK4_HELLOWORLDHTTPSERVER_H
