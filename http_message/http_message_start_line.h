#ifndef TASK4_HTTP_MESSAGE_START_LINE_H
#define TASK4_HTTP_MESSAGE_START_LINE_H

#include <string>
#include <vector>

class http_message_request;

class http_message_start_line {
public:
    std::vector<char> method_;
    std::vector<char> target_;
    int ver_before_point_;
    int ver_after_point_;

    http_message_start_line() = default;

    http_message_start_line(const std::vector<char> &method, const std::vector<char> &target, int verBeforePoint,
                            int verAfterPoint);
};


#endif //TASK4_HTTP_MESSAGE_START_LINE_H
