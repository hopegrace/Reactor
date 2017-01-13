#include "HttpServer.h"
#include <iostream>
#include "../TcpConnection.h"
#include "../../base/SimpleLogger.h"
#include "HttpHandler.h"
#include "HttpResponse.h"

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
		clients_.insert(std::make_pair(conn, HttpRequest(conn)));
	} else if (clients_.count(conn)) {
		clients_.erase(conn);
	}
}

void HttpServer::on_message(const TcpConnectionPtr &conn) {
	auto it = clients_.find(conn);
	assert(it != clients_.end());
	HttpRequest &request = it->second;
	assert(!request.finished());
	LOG(Debug) << conn->peer_address().to_string() << " message";

	request.parse();
	if (request.error()) {
		// TODO send_error
		conn->close();
		clients_.erase(conn);
	} else if (request.finished()) {
		HttpResponse response;
		handler_->request(request, &response);
		response.send(conn);
		conn->close();
		clients_.erase(conn);
	} 
}


} // namespace http
} // namespace net
} // namespace reactor
