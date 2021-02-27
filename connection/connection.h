#ifndef TASK4_CONNECTION_H
#define TASK4_CONNECTION_H

#include <array>
#include <memory>
#include <cstddef>
#include <boost/asio.hpp>

#include "../http_server/http_server.h"
#include "../http_message/http_message_parser.h"
#include "../http_message/http_message_request.h"
#include "../http_message/http_message_reply.h"

using namespace boost::asio;

class http_server;

class connection : public std::enable_shared_from_this<connection> {
public:
    connection(http_server &server,
               ip::tcp::socket sock);

    const http_message_request &get_req() const;

    const std::vector<char> &get_body() const;

    void do_read();

    template<typename HandlerType>
    void prepare_reply(HandlerType handler) {
        handler(req_, reply_);
    }

    void do_write();

private:

    void read_request();

    void read_body(std::size_t content_len);

    http_server &server_;
    ip::tcp::socket sock_;
    std::array<char, 8192> buffer_;
    http_message_parser http_message_parser_;

    http_message_request req_;
    std::vector<char> body_;

    http_message_reply reply_;

};


#endif //TASK4_CONNECTION_H
