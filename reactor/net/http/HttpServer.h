#ifndef REACTOR_EXAMPLE_HTTP_SERVER_H
#define REACTOR_EXAMPLE_HTTP_SERVER_H

#include "../TcpServer.h"
#include "HttpRequest.h"

namespace reactor {
namespace net {
namespace http {

class HttpHandler;

class HttpServer {
public:
	HttpServer(EventLoop *loop, const InetAddress &addr, HttpHandler *handler):
		loop_(loop), server_(loop, addr), handler_(handler), clients_() {

		using namespace std::placeholders;
		server_.set_connection_callback(std::bind(&HttpServer::on_connection, this, _1));
		server_.set_message_callback(std::bind(&HttpServer::on_message, this, _1));
	}

	~HttpServer() = default;

	HttpServer(const HttpServer &) = delete;
	HttpServer &operator=(const HttpServer &) = delete;

	void start() 
	{ server_.start(); }

private:
	void on_connection	(const TcpConnectionPtr &conn);
	void on_message		(const TcpConnectionPtr &conn);

	typedef std::unordered_map<TcpConnectionPtr, HttpRequest> ClientMap;

	EventLoop   *loop_;
	TcpServer   server_;
	HttpHandler *handler_;
	ClientMap   clients_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
