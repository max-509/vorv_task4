#include "HelloWorldHttpServer.h"

HelloWorldHttpServer::HelloWorldHttpServer(unsigned short port) : http_server(port) { }

void HelloWorldHttpServer::start() {
    ip::tcp::acceptor ac{service_};
    ac.open(ep_.protocol());
    ac.set_option(ip::tcp::acceptor::reuse_address(true));
    ac.bind(ep_);
    ac.listen();
    ac.async_accept([this, &ac](boost::system::error_code ec, ip::tcp::socket socket) {
        if (!ac.is_open()) {
            return;
        }
        if (!ec) {
            std::shared_ptr<connection> con_ptr(new connection(*this, std::move(socket)));
            con_ptr->do_read();
        }
    });
    service_.run();
}

void HelloWorldHttpServer::stop(std::shared_ptr<connection> con_ptr) {

}

void HelloWorldHttpServer::after_read_handler(std::shared_ptr<connection> con_ptr) {
    con_ptr->prepare_reply([this](const http_message_request &req, http_message_reply &reply) {
        reply.ver_before_point_ = req.start_line_.ver_before_point_;
        reply.ver_after_point_ = req.start_line_.ver_after_point_;
        reply.status_ = http_message_reply::status::OK;

        http_message_header content_length_header;
        content_length_header.name_ = "Content-Length";
        content_length_header.value_ = std::to_string(this->HELLO_WORLD_PAGE.size());

        http_message_header content_type_header;
        content_type_header.name_ = "Content-Type";
        content_type_header.value_ = "text/html; charset=UTF-8";

        reply.headers_.push_back(content_length_header);
        reply.headers_.push_back(content_type_header);

        reply.body_.insert(reply.body_.end(), this->HELLO_WORLD_PAGE.begin(), this->HELLO_WORLD_PAGE.end());
    });

    con_ptr->do_write();
}

void HelloWorldHttpServer::stop() { service_.stop(); }
