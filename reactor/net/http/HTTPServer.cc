#include "HTTPServer.h"
#include "../TcpConnection.h"
#include "../../base/SimpleLogger.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif 

namespace reactor {
namespace net {
namespace http {

void HTTPServer::on_connection(const TcpConnectionPtr &conn) {
	bool connected = conn->connected();
	LOG(Debug) << conn->peer_address().to_string() << (connected ? " connected" : " disconnected");
	if (connected) {
		assert(clients_.find(conn) == clients_.end());
		clients_.insert(std::make_pair(conn, HTTPRequest()));
	} else {
		assert(clients_.find(conn) != clients_.end());
		clients_.erase(conn);
	}
}

void HTTPServer::on_message(const TcpConnectionPtr &conn) {
	//auto it = clients_.find(conn);
	//assert(it != clients_.end());
	//HTTPRequest &request = it->second;

	//char *data = conn->buffer()->data();
	//char *end = strstr(data, "\r\n\r\n");
	// TODO state-machine parse
	//if (!end) {
//		return;
	//}
	// TODO
}

} // namespace http
} // namespace net
} // namespace reactor
