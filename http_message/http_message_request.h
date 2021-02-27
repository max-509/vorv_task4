#ifndef TASK4_HTTP_MESSAGE_REQUEST_H
#define TASK4_HTTP_MESSAGE_REQUEST_H

#include <vector>

#include "http_message_header.h"
#include "http_message_start_line.h"

//class http_message_start_line;

class http_message_request {
public:

    http_message_start_line start_line_;
    std::vector<http_message_header> headers_;

    http_message_request() = default;

    http_message_request(const http_message_start_line &start_line, const std::vector<http_message_header> &headers);
};


#endif //TASK4_HTTP_MESSAGE_REQUEST_H
