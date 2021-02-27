#include "ConsoleHttpServer.h"

ConsoleHttpServer::ConsoleHttpServer(unsigned short port) : http_server(port) { }

void ConsoleHttpServer::start() {
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

void ConsoleHttpServer::stop(std::shared_ptr<connection> con_ptr) {

}

void ConsoleHttpServer::after_read_handler(std::shared_ptr<connection> con_ptr) {
    print_request(con_ptr->get_req(), con_ptr->get_body());
}

void ConsoleHttpServer::print_request(const http_message_request &req,
                                      const std::vector<char> &body) {
    std::cout << std::string(req.start_line_.method_.begin(), req.start_line_.method_.end());
    std::cout << " ";
    std::cout << std::string(req.start_line_.target_.begin(), req.start_line_.target_.end());
    std::cout << " HTTP/" << req.start_line_.ver_before_point_ << '.' << req.start_line_.ver_after_point_ << "\r\n";
    for (const http_message_header &header : req.headers_) {
        std::cout << header.name_ << ": " << header.value_ << "\r\n";
    }
    std::cout << "\r\n";
    std::for_each(body.begin(), body.end(), [](const char &c) { std::cout << c; });
}

void ConsoleHttpServer::stop() { service_.stop(); }
