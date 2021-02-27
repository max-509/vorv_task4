#include "http_server.h"

http_server::http_server(unsigned short port) : ep_(ip::address_v4::any(), port) {  }
