#include "http_message_parser.h"

#include <cctype>

void http_message_parser::reset() {
    parsing_state_ = parsing_state::start;
}

http_message_parser::parsing_result http_message_parser::parsing_char(http_message_request &request, char c) {
    switch(parsing_state_) {
        case parsing_state::start: {
            if (!isalpha(c)) {
                return parsing_result::bad;
            }
            parsing_state_ = parsing_state::method;
            request.start_line_.method_.push_back(c);
            return parsing_result::intermediate;
        }
        case parsing_state::method: {
            if (c == ' ') {
                parsing_state_ = parsing_state::target;
            } else if (!isalpha(c)) {
                return parsing_result::bad;
            } else {
                parsing_state_ = parsing_state::method;
                request.start_line_.method_.push_back(c);
            }
            return parsing_result::intermediate;
        }
        case parsing_state::target: {
            if (c == ' ') {
                parsing_state_ = parsing_state::http_prev_version;
            } else if (iscntrl(c)) {
                return parsing_result::bad;
            } else {
                parsing_state_ = parsing_state::target;
                request.start_line_.target_.push_back(c);
            }
            return parsing_result::intermediate;
        }
        case parsing_state::http_prev_version: {
            if (c == '/') {
                parsing_state_ = parsing_state::http_ver_before_point;
            } else if (!isalpha(c)) {
                return parsing_result::bad;
            } else {
                parsing_state_ = parsing_state::http_prev_version;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::http_ver_before_point: {
            if (c == '.') {
                parsing_state_ = parsing_state::http_ver_after_point;
            } else if (!isdigit(c)) {
                return parsing_result::bad;
            } else {
                request.start_line_.ver_before_point_ = c - '0';
                parsing_state_ = parsing_state::http_ver_before_point;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::http_ver_after_point: {
            if (c == '\r') {
                parsing_state_ = parsing_state::http_prev_header;
            } else if (!isdigit(c)) {
                return parsing_result::bad;
            } else {
                request.start_line_.ver_after_point_ = c - '0';
                parsing_state_ = parsing_state::http_ver_after_point;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::http_prev_header: {
            if (c == '\n') {
                parsing_state_ = parsing_state::headers_start;
            } else {
                return parsing_result::bad;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::headers_start: {
            if (c == '\r') {
                parsing_state_ = parsing_state::empty_line1;
            } else {
                parsing_state_ = parsing_state::header_name;
                request.headers_.push_back(http_message_header());
                request.headers_.back().name_ += c;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::header_name: {
            if (c == ':') {
                parsing_state_ = parsing_state::header_space;
            } else {
                parsing_state_ = parsing_state::header_name;
                request.headers_.back().name_ += c;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::header_space: {
            if (c == ' ') {
                parsing_state_ = parsing_state::header_val;
            } else {
                return parsing_result::bad;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::header_val: {
            if (c == '\r') {
                parsing_state_ = parsing_state::http_prev_header;
            } else {
                parsing_state_ = parsing_state::header_val;
                request.headers_.back().value_ += c;
            }
            return parsing_result::intermediate;
        }
        case parsing_state::empty_line1: {
            if (c == '\n') {
                return parsing_result::ok;
            } else {
                return parsing_result::bad;
            }

        }
    }
}
