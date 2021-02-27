#ifndef TASK4_THREADPOOLHTTPSERVER_H
#define TASK4_THREADPOOLHTTPSERVER_H

#include <string>
#include <cstddef>
#include <memory>
#include <random>
#include <ctime>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>

#include "../http_server/http_server.h"
#include "../connection/connection.h"

using namespace boost::placeholders;
using namespace boost::asio;

class ThreadPoolHttpServer : public http_server {
public:
    explicit ThreadPoolHttpServer(unsigned short port);

    ThreadPoolHttpServer(unsigned short port, std::size_t n_threads);

    void start() override;

    void stop(std::shared_ptr<connection> con_ptr) override;

    void stop() override;

    void after_read_handler(std::shared_ptr<connection> con_ptr) override;
private:
    void signal_handler(const boost::system::error_code & err, int signal);

    void accept_handler(std::shared_ptr<ip::tcp::acceptor> ac_ptr, const boost::system::error_code &ec);

    std::string random_string(int len);

    static constexpr int MIN_SIZE = 100;
    static constexpr int MAX_SIZE = 100000;

    std::string before_text_ = "<!DOCTYPE html>\n"
                               "<html lang=\"en\">\n"
                               "\n"
                               "    <head>\n"
                               "        <meta charset=\"UTF-8\">\n"
                               "        <title>TEXT</title>\n"
                               "    </head>\n"
                               "\n"
                               "    <body>\n"
                               "        <p>";
    std::string after_text_ = "</p>\n"
                               "    </body>\n"
                               "\n</html>";
    std::mt19937 gen_{std::chrono::high_resolution_clock::now().time_since_epoch().count()};
    std::uniform_int_distribution<int> uni_{MIN_SIZE, MAX_SIZE};
    std::size_t n_threads_;
    signal_set signals_;
    ip::tcp::socket listen_socket_;
    boost::thread_group thread_pool_;

};

#endif //TASK4_THREADPOOLHTTPSERVER_H