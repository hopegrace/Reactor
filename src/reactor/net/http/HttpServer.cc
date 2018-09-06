#include "reactor/net/http/HttpServer.h"
#include <iostream>
#include "reactor/base/SimpleLogger.h"
#include "reactor/net/TcpConnection.h"
#include "reactor/net/http/HttpHandler.h"
#include "reactor/net/http/HttpResponse.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif 

using namespace std;

namespace reactor {
namespace net {
namespace http {

void HttpServer::on_connection(const TcpConnectionPtr &conn) {
	bool connected = conn->connected();
	LOG(Debug) << conn->peer_address().to_string() << (connected ? " connected" : " disconnected");
	if (connected) {
		assert(clients_.find(conn) == clients_.end());
		clients_.insert(std::make_pair(conn, HttpContext()));
	} else if (clients_.count(conn)) {
		clients_.erase(conn);
	}
}

void HttpServer::on_message(const TcpConnectionPtr &conn) {
	auto it = clients_.find(conn);
	assert(it != clients_.end());
	HttpContext &context = it->second;
	assert(!context.finished());
	LOG(Debug) << conn->peer_address().to_string() << " message";

	context.parse(conn);
	if (context.error()) {
		char buff[20];
		snprintf(buff, sizeof buff, "%u ", context.error_code());
		conn->write(buff);
		conn->write(context.error_message());
	} else if (context.finished()) {
		handler_->request(context.request(), &context.response());
		context.response().send(conn);
	} else {
		return;
	}

	if (context.close_connection()) {
		clients_.erase(conn);
		conn->close();
	} else {
		context.reset();
	}
}


} // namespace http
} // namespace net
} // namespace reactor
