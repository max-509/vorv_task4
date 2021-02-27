#include "http_message_request.h"

http_message_request::http_message_request(const http_message_start_line &start_line, const std::vector<http_message_header> &headers)
        : start_line_(start_line), headers_(headers) {}
