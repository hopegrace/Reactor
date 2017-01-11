#ifndef REACTOR_NET_HTTP_BASE_HANDLER_H
#define REACTOR_NET_HTTP_BASE_HANDLER_H

namespace reactor {
namespace net {
namespace http {

class HTTPRequest;
class HTTPResponse;
class HTTPServer;

class HTTPHandler {
public:
	friend class HTTPServer;

	virtual void GET  (const HTTPRequest &request, HTTPResponse *response);
	virtual void POST (const HTTPRequest &request, HTTPResponse *response);
	virtual void HEAD (const HTTPRequest &request, HTTPResponse *response);

private:
	// for HTTPServer use
	void request(const HTTPRequest &request, HTTPResponse *response);
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
