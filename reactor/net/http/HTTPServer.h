#ifndef REACTOR_EXAMPLE_HTTP_SERVER_H
#define REACTOR_EXAMPLE_HTTP_SERVER_H

#include "../TcpServer.h"

namespace reactor {
namespace net {
namespace http {

class BaseHandler;

class HTTPServer {
public:
	HTTPServer(EventLoop *loop, const InetAddress &addr):
		loop_(loop), 
		server_(loop, addr) {

		using namespace std::placeholders;
		server_.set_connection_callback(std::bind(&HTTPServer::on_connection, this, _1));
		server_.set_message_callback(std::bind(&HTTPServer::on_message, this, _1));
	}

	void start() { server_.start(); }

private:
	void on_connection	(const TcpConnectionPtr &conn);
	void on_message		(const TcpConnectionPtr &conn);

	EventLoop *loop_;
	TcpServer server_;
	BaseHandler *handler_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
