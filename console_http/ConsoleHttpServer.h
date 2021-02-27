#ifndef TASK4_CONSOLEHTTPSERVER_H
#define TASK4_CONSOLEHTTPSERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <vector>

#include "../http_message/http_message_parser.h"
#include "../http_message/http_message_request.h"
#include "../http_server/http_server.h"
#include "../connection/connection.h"

using namespace boost::asio;

using socket_ptr = std::shared_ptr<ip::tcp::socket>;

class ConsoleHttpServer : public http_server {
private:

    void print_request(const http_message_request &req,
                       const std::vector<char> &body);

public:
    explicit ConsoleHttpServer(unsigned short port);

    void start() override;

    void stop(std::shared_ptr<connection> con_ptr) override;

    void stop() override;

    void after_read_handler(std::shared_ptr<connection> con_ptr) override;
};


#endif //TASK4_CONSOLEHTTPSERVER_H
