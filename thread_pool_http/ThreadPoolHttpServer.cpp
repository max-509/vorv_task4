#include "ThreadPoolHttpServer.h"

#include <iostream>

ThreadPoolHttpServer::ThreadPoolHttpServer(unsigned short port) : http_server(port),
                                                                listen_socket_(service_),
                                                                signals_(service_) {
    n_threads_ = 8;
}

ThreadPoolHttpServer::ThreadPoolHttpServer(unsigned short port, std::size_t n_threads) : http_server(port),
                                                                                        listen_socket_(service_),
                                                                                        signals_(service_),
                                                                                        n_threads_(n_threads)
{ }

void ThreadPoolHttpServer::start() {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&ThreadPoolHttpServer::signal_handler, this, placeholders::error, boost::placeholders::_2));

    std::shared_ptr<ip::tcp::acceptor> ac_ptr{new ip::tcp::acceptor{service_}};
    ac_ptr->open(ep_.protocol());
    ac_ptr->set_option(ip::tcp::acceptor::reuse_address(true));
    ac_ptr->bind(ep_);
    ac_ptr->listen(socket_base::max_listen_connections);
    ac_ptr->async_accept(listen_socket_, boost::bind(&ThreadPoolHttpServer::accept_handler, this, ac_ptr, placeholders::error));
    for (std::size_t i = 0; i < n_threads_; ++i) {
        thread_pool_.create_thread([this]() {
            service_.run();
        });
    }
    thread_pool_.join_all();
}

void ThreadPoolHttpServer::stop(std::shared_ptr<connection> con_ptr) { }

void ThreadPoolHttpServer::stop() {
    service_.stop();
}

void ThreadPoolHttpServer::after_read_handler(std::shared_ptr<connection> con_ptr) {
    con_ptr->prepare_reply([this](const http_message_request &req, http_message_reply &reply) {
        reply.ver_before_point_ = req.start_line_.ver_before_point_;
        reply.ver_after_point_ = req.start_line_.ver_after_point_;
        reply.status_ = http_message_reply::status::OK;

        auto rand_size = this->uni_(this->gen_);

        http_message_header content_length_header;
        content_length_header.name_ = "Content-Length";
        content_length_header.value_ = std::to_string(rand_size);

        http_message_header content_type_header;
        content_type_header.name_ = "Content-Type";
        content_type_header.value_ = "text/html; charset=UTF-8";

        reply.headers_.push_back(content_length_header);
        reply.headers_.push_back(content_type_header);

        auto rand_string = this->random_string(rand_size);

        reply.body_.insert(reply.body_.end(), this->before_text_.begin(), this->before_text_.end());
        reply.body_.insert(reply.body_.end(), rand_string.begin(), rand_string.end());
        reply.body_.insert(reply.body_.end(), this->after_text_.begin(), this->after_text_.end());
    });
    con_ptr->do_write();
}

std::string ThreadPoolHttpServer::random_string(int len) {
    srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    auto randchar = []() {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(len, 0);
    std::generate_n(str.begin(), len, randchar);
    return str;
}

void ThreadPoolHttpServer::signal_handler(const boost::system::error_code & err, int signal) {
    stop();
}

void ThreadPoolHttpServer::accept_handler(std::shared_ptr<ip::tcp::acceptor> ac_ptr, const boost::system::error_code &ec) {
    if (!ac_ptr->is_open()) {
        return;
    }
    if (!ec) {
        std::shared_ptr<connection> con_ptr(new connection(*this, std::move(listen_socket_)));
        con_ptr->do_read();
    }
    ac_ptr->async_accept(listen_socket_, boost::bind(&ThreadPoolHttpServer::accept_handler, this, ac_ptr, placeholders::error));
}