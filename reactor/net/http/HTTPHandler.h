#ifndef REACTOR_NET_HTTP_BASE_HANDLER_H
#define REACTOR_NET_HTTP_BASE_HANDLER_H

namespace reactor {
namespace net {
namespace http {

class HTTPRequest;
class HTTPResponse;

class HTTPHandler {
public:
	virtual void GET  (const HTTPRequest &request, HTTPResponse *response) = 0;
	virtual void POST (const HTTPRequest &request, HTTPResponse *response) = 0;
	virtual void HEAD (const HTTPRequest &request, HTTPResponse *response) = 0;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
