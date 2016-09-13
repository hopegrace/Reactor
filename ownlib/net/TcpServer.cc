#include <ownlib/net/TcpServer.h>

#include <assert.h>
#include <iostream>
#include <unistd.h>

#include <ownlib/base/SimpleLogger.h>
#include <ownlib/net/Channel.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/TcpConnection.h>
#include <ownlib/net/TcpSocket.h>

using namespace std;
using namespace sduzh::base;

namespace sduzh {
namespace net {

TcpServer::TcpServer(EventLoop *loop):
		loop_(loop),
		bind_socket_(new TcpSocket()),
		bind_channel_(new Channel(loop, bind_socket_->fd())) {
	bind_socket_->set_blocking(false);
	bind_socket_->set_reuse_addr(true);
	using namespace std::placeholders;
	bind_channel_->set_read_callback(std::bind(&TcpServer::on_connection, this));
}

TcpServer::~TcpServer() {
	delete bind_channel_;
	delete bind_socket_;
}

void TcpServer::set_reuse_addr(bool on) {
	bind_socket_->set_reuse_addr(on);
}

void TcpServer::start(const InetAddress &bind_addr) {
	bind_socket_->bind(bind_addr);
	bind_socket_->listen(5);
}

void TcpServer::on_connection() {
	InetAddress client_addr;
	int client = bind_socket_->accept(&client_addr);
	LOG(Info) << client_addr.host() << ":" << client_addr.port() << " connected\n";	
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
		cout << "no connection callback, close it\n";
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
} // namespace sduzh


