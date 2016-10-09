#include <reactor/net/TcpClient.h>

#include <functional>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/InetAddress.h>

namespace reactor {
namespace net {

TcpClient::TcpClient(EventLoop *loop, const InetAddress &servaddr):
	loop_(loop),
	connector_(new Connector(loop, servaddr)) {

	using namespace std::placeholders;
	connector_->set_connection_callback(std::bind(&TcpClient::on_connected, this, _1));
}

void TcpClient::connect() {
	connector_->start();
}

void TcpClient::disconnect() {
	// TODO
}

void TcpClient::stop() {
	// TODO
}

void TcpClient::on_connected(int fd) {
	assert(!connection_);
	connection_.reset(new TcpConnection(loop_, fd));
	connection_->set_connection_callback(connection_cb_);
	connection_->set_message_callback(message_cb_);
	connection_->set_write_complete_callback(write_complete_cb_);
	using namespace std::placeholders;
	connection_->set_close_callback(std::bind(&TcpClient::on_close, this, _1));
	connection_->connection_established();
}

void TcpClient::on_close(const TcpConnectionPtr &conn) {
	assert(connection_ == conn);
	(void)conn;
	connection_.reset();
}

} // namespace net
} // namespace reactor
