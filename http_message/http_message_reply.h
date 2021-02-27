#ifndef TASK4_HTTP_MESSAGE_REPLY_H
#define TASK4_HTTP_MESSAGE_REPLY_H

#include <vector>
#include <array>
#include <string>

#include "http_message_header.h"

class http_message_reply {
public:
    enum class status {
        CONTINUE = 100,
        SWITCHING_PROTOCOLS = 101,
        PROCESSING = 102,
        EARLY_HINTS = 103,
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NON_AUTHORITATIVE_INFORMATION = 203,
        NO_CONTENT = 204,
        RESET_CONTENT = 205,
        PARTIAL_CONTENT = 206,
        MULTI_STATUS = 207,
        ALREADY_REPORTED = 208,
        IM_USED = 226,
        MULTIPLE_CHOICES = 300,
        MOVED_PERMANENTLY = 301,
        MOVED_TEMPORARILY = 302,
        FOUND = 302,
        SEE_OTHER = 303,
        NOT_MODIFIED = 304,
        USE_PROXY = 305,
        TEMPORARY_REDIRECT = 307,
        PERMANENT_REDIRECT = 308,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        PAYMENT_REQUIRED = 402,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        NOT_ACCEPTABLE = 406,
        PROXY_AUTHENTICATION_REQUIRED = 407
    };

    int ver_before_point_;
    int ver_after_point_;
    status status_;
    std::vector<http_message_header> headers_;
    std::vector<char> body_;

    std::vector<char> &to_buffer();

    std::vector<char> buffer;

    std::array<char, 5> HTTP_CHAR = {'H', 'T', 'T', 'P', '/'};
};


#endif //TASK4_HTTP_MESSAGE_REPLY_H
