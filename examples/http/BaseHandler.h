#ifndef REACTOR_EXAMPLE_HTTP_BASE_HANDLER_H
#define REACTOR_EXAMPLE_HTTP_BASE_HANDLER_H

namespace reactor {
namespace net {
namespace http {

class BaseHandler {
public:
	virtual void GET  (const Request &request, Response *response) = 0;
	virtual void POST (const Request &request, Response *response) = 0;
	virtual void HEAD (const Request &request, Response *response) = 0;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
