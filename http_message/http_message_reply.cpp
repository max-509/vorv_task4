#include "http_message_reply.h"

std::vector<char> & http_message_reply::to_buffer() {
    buffer.insert(buffer.end(), HTTP_CHAR.begin(), HTTP_CHAR.end());
    buffer.push_back(static_cast<char>(ver_before_point_) + '0');
    buffer.push_back('.');
    buffer.push_back(static_cast<char>(ver_after_point_) + '0');
    buffer.push_back(' ');
    std::string status_str = std::to_string(static_cast<int>(status_));
    buffer.insert(buffer.end(), status_str.begin(), status_str.end());
    buffer.push_back(' ');
    std::string status_name;
    if (status_ == status::OK) {
        status_name = "OK";
        buffer.insert(buffer.end(), status_name.begin(), status_name.end());
    } else {
        status_name = "Bad Request";
        buffer.insert(buffer.end(), status_name.begin(), status_name.end());
    }
    buffer.push_back('\r');
    buffer.push_back('\n');
    for (const auto &header : headers_) {
        buffer.insert(buffer.end(), header.name_.begin(), header.name_.end());
        buffer.push_back(':');
        buffer.push_back(' ');
        buffer.insert(buffer.end(), header.value_.begin(), header.value_.end());
        buffer.push_back('\r');
        buffer.push_back('\n');
    }
    buffer.push_back('\r');
    buffer.push_back('\n');
    buffer.insert(buffer.end(), body_.begin(), body_.end());

    return buffer;
}