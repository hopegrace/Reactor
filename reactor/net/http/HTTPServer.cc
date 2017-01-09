#include "HTTPServer.h"
#include "../TcpConnection.h"
#include "../../base/SimpleLogger.h"

namespace reactor {
namespace net {
namespace http {

void HTTPServer::on_connection(const TcpConnectionPtr &conn) {
	const char *msg = conn->connected() ? " connected" : " disconnected";
	LOG(Debug) << conn->peer_address().to_string() << msg;
}

void HTTPServer::on_message(const TcpConnectionPtr &conn) {
	LOG(Debug) << conn->buffer()->data();
	conn->buffer()->clear();
}

} // namespace http
} // namespace net
} // namespace reactor
