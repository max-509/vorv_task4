#ifndef TASK4_HTTP_SERVER_H
#define TASK4_HTTP_SERVER_H

#include <boost/asio.hpp>
#include <memory>

#include "../connection/connection.h"

using namespace boost::asio;

class connection;

class http_server {
public:
    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void stop(std::shared_ptr<connection> con_ptr) = 0;

    virtual void after_read_handler(std::shared_ptr<connection> con_ptr) = 0;

    http_server(unsigned short port);
protected:
    io_service service_{};
    ip::tcp::endpoint ep_;
};


#endif //TASK4_HTTP_SERVER_H
