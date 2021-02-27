#include "http_message_header.h"

http_message_header::http_message_header(const std::string &name, const std::string &value) : name_(name),
                                                                                              value_(value) {}
