#include "connection.h"

#include <iostream>

connection::connection(http_server &server, ip::tcp::socket sock) : server_(server), sock_(std::move(sock)) { }

void connection::do_read() {
    read_request();
}

void connection::read_request() {
    auto self(shared_from_this());
    sock_.async_read_some(buffer(buffer_), [this, self](boost::system::error_code ec, std::size_t len) {
        if (!ec) {
            auto res = http_message_parser_.parsing_part(req_, buffer_.begin(), buffer_.begin() + len);

            if (res.first == http_message_parser::parsing_result::intermediate) {
                read_request();
            } else if (res.first == http_message_parser::parsing_result::bad) {
                server_.stop(self);
            } else if (res.first == http_message_parser::parsing_result::ok) {
                auto content_length_it = std::find_if(req_.headers_.begin(), req_.headers_.end(), [](const http_message_header &header)
                { return header.name_ == "Content-Length"; });
                if (content_length_it != req_.headers_.end()) {
                    std::stringstream content_length_stream(content_length_it->value_);
                    std::size_t content_length;
                    content_length_stream >> content_length;
                    body_.insert(body_.end(), res.second, buffer_.begin() + len);
                    content_length -= ((buffer_.begin() + len) - res.second);

                    read_body(content_length);
                } else {
                    server_.after_read_handler(self);
                }
            }
        } else {
            server_.stop(self);
        }
    });
}

void connection::read_body(std::size_t content_len) {
    auto self(shared_from_this());
    sock_.async_read_some(buffer(buffer_), [this, self, content_len](boost::system::error_code ec, std::size_t len) {
        if (!ec) {
            if (content_len > 0) {
                body_.insert(body_.end(), buffer_.begin(), buffer_.begin() + len);
                read_body(content_len - len);
            } else {
                server_.after_read_handler(self);
            }

        } else {
            server_.stop(self);
        }
    });
}

void connection::do_write() {
    auto self(shared_from_this());
    async_write(sock_, buffer(reply_.to_buffer()), [this, self](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            boost::system::error_code ignored_ec;
            sock_.shutdown(ip::tcp::socket::shutdown_both,
                             ignored_ec);
        } else {
            server_.stop(self);
        }
    });
}

const http_message_request &connection::get_req() const {
    return req_;
}

const std::vector<char> &connection::get_body() const {
    return body_;
}
