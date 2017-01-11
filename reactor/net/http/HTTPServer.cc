#include "HTTPServer.h"
#include <iostream>
#include "../TcpConnection.h"
#include "../../base/SimpleLogger.h"
#include "HTTPHandler.h"
#include "HTTPResponse.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif 

using namespace std;

namespace reactor {
namespace net {
namespace http {

void HTTPServer::on_connection(const TcpConnectionPtr &conn) {
	bool connected = conn->connected();
	LOG(Debug) << conn->peer_address().to_string() << (connected ? " connected" : " disconnected");
	if (connected) {
		assert(clients_.find(conn) == clients_.end());
		clients_.insert(std::make_pair(conn, HTTPRequest()));
	} else if (clients_.count(conn)) {
		clients_.erase(conn);
	}
}

void HTTPServer::on_message(const TcpConnectionPtr &conn) {
	assert(clients_.find(conn) != clients_.end());
	HTTPRequest &request = clients_[conn];
	assert(!request.finished());

	conn->buffer()->retrieve(request.parse(conn->buffer()->as_str()));
	if (request.error()) {
		// send_error
		conn->close();
		clients_.erase(conn);
	} else if (request.finished()) {
		HTTPResponse response(conn);
		handler_->request(request, &response);
		conn->close();
		clients_.erase(conn);
	} 
}


} // namespace http
} // namespace net
} // namespace reactor
