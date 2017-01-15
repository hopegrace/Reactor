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

	virtual void doGet  (const HttpRequest &request, HttpResponse *response);
	virtual void doPost (const HttpRequest &request, HttpResponse *response);
	virtual void doHead (const HttpRequest &request, HttpResponse *response);

private:
	// for HttpServer use
	void request(const HttpRequest &request, HttpResponse *response);
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
