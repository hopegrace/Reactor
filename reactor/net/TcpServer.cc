#include <reactor/net/TcpServer.h>

#include <assert.h>
#include <iostream>
#include <unistd.h>

#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Channel.h>
#include <reactor/net/TcpConnection.h>
#include <reactor/net/TcpSocket.h>

using namespace std;
using namespace reactor::base;

namespace reactor {
namespace net {

TcpServer::TcpServer(EventLoop *loop, const InetAddress &addr):
		loop_(loop),
		acceptor_(loop, addr),
		started_(false) {

	using namespace std::placeholders;
	acceptor_.set_new_connection_callback(std::bind(&TcpServer::on_connection, this, _1, _2, _3));
}

TcpServer::~TcpServer() {
}

void TcpServer::start() {
	if (!started_) {
		acceptor_.listen();
		started_ = true;
	}
}

void TcpServer::on_connection(int client, const InetAddress &client_addr, Timestamp time) {
	if (connection_cb_) {
		assert(connections_.find(client) == connections_.end());
		TcpConnectionPtr conn = std::make_shared<TcpConnection>(loop_, client);
		
		using namespace std::placeholders;
		conn->set_connection_callback(connection_cb_);
		conn->set_message_callback(message_cb_);
		conn->set_write_complete_callback(write_complete_cb_);
		conn->set_close_callback(std::bind(&TcpServer::on_close, this, _1));

		connections_[client] = conn;

		conn->connection_established();
	} else {
		cout << "no connection callback, close it";
		::close(client);
	}
}

void TcpServer::on_close(const TcpConnectionPtr &conn) {
	assert(connections_[conn->fd()] == conn);
	InetAddress peer = conn->peer_address();
	LOG(Info) << peer.host() << ":" << static_cast<long>(peer.port()) << " closed";
	connections_.erase(conn->fd());
}

} // namespace net
} // namespace reactor


