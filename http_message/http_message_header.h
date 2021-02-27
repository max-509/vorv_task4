#ifndef TASK4_HTTP_MESSAGE_HEADER_H
#define TASK4_HTTP_MESSAGE_HEADER_H


#include <string>

class http_message_header {
public:
    std::string name_ = "";
    std::string value_ = "";

    http_message_header() = default;

    http_message_header(const std::string &name, const std::string &value);

};


#endif //TASK4_HTTP_MESSAGE_HEADER_H
