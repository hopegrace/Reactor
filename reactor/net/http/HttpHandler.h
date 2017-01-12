#ifndef REACTOR_NET_HTTP_BASE_HANDLER_H
#define REACTOR_NET_HTTP_BASE_HANDLER_H

namespace reactor {
namespace net {
namespace http {

class HttpRequest;
class HttpResponse;
class HttpServer;

class HttpHandler {
public:
	friend class HttpServer;

	virtual void GET  (const HttpRequest &request, HttpResponse *response);
	virtual void POST (const HttpRequest &request, HttpResponse *response);
	virtual void HEAD (const HttpRequest &request, HttpResponse *response);

private:
	// for HttpServer use
	void request(const HttpRequest &request, HttpResponse *response);
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
