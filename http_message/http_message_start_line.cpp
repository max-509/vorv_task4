#include "http_message_start_line.h"

#include "http_message_request.h"

http_message_start_line::http_message_start_line(const std::vector<char> &method, const std::vector<char> &target,
                                                 int verBeforePoint, int verAfterPoint) : method_(method),
                                                                                          target_(target),
                                                                                          ver_before_point_(
                                                                                                  verBeforePoint),
                                                                                          ver_after_point_(
                                                                                                  verAfterPoint) {}
