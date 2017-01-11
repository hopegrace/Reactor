#ifndef REACTOR_NET_HTTP_STATUS_H
#define REACTOR_NET_HTTP_STATUS_H

#include <string>

namespace reactor {
namespace net {
namespace http {

std::string status_text(int status_code);


static const int StatusOk       = 200;  // Ok
static const int StatusNotFound = 404;  // Not Found

} // namespace http
} // namespace net
} // namespace reactor

#endif
