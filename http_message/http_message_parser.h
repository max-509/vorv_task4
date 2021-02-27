#ifndef TASK4_HTTP_MESSAGE_PARSER_H
#define TASK4_HTTP_MESSAGE_PARSER_H

#include <utility>

#include "http_message_request.h"

class http_message_parser {
public:

    enum class parsing_result {
        ok, bad, intermediate
    };

    void reset();

    template<typename InputIt>
    std::pair<parsing_result, InputIt> parsing_part(http_message_request &request, InputIt begin, InputIt end) {
        parsing_result res;
        for (InputIt i = begin; i != end; ++i) {
            res = parsing_char(request, *i);
            if (res == parsing_result::ok || res == parsing_result::bad) {
                return std::make_pair(res, ++i);
            }
        }
        return std::make_pair(res, end);
    }
private:
    enum class parsing_state {
        start,
        method,
        target,
        http_prev_version,
        http_ver_before_point,
        http_ver_after_point,
        http_prev_header,
        headers_start,
        header_name,
        header_space,
        header_val,
        empty_line1
    } parsing_state_ = parsing_state::start;

    parsing_result parsing_char(http_message_request &request, char c);
};


#endif //TASK4_HTTP_MESSAGE_PARSER_H
