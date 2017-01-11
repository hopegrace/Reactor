#ifndef REACTOR_EXAMPLE_HTTP_SERVER_H
#define REACTOR_EXAMPLE_HTTP_SERVER_H

#include "../TcpServer.h"
#include "HTTPRequest.h"

namespace reactor {
namespace net {
namespace http {

class HTTPHandler;

class HTTPServer {
public:
	HTTPServer(EventLoop *loop, const InetAddress &addr, HTTPHandler *handler):
		loop_(loop), server_(loop, addr), handler_(handler), clients_() {

		using namespace std::placeholders;
		server_.set_connection_callback(std::bind(&HTTPServer::on_connection, this, _1));
		server_.set_message_callback(std::bind(&HTTPServer::on_message, this, _1));
	}

	~HTTPServer() = default;

	HTTPServer(const HTTPServer &) = delete;
	HTTPServer &operator=(const HTTPServer &) = delete;

	void start() { server_.start(); }
	void stop() { /* TODO */ }

private:
	void on_connection	(const TcpConnectionPtr &conn);
	void on_message		(const TcpConnectionPtr &conn);

	typedef std::unordered_map<TcpConnectionPtr, HTTPRequest> ClientMap;

	EventLoop   *loop_;
	TcpServer   server_;
	HTTPHandler *handler_;
	ClientMap   clients_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
