#include <ownlib/net/TcpServer.h>

#include <assert.h>
#include <iostream>
#include <unistd.h>

#include <ownlib/net/Channel.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/TcpConnection.h>
#include <ownlib/net/TcpSocket.h>

using namespace std;

namespace sduzh {
namespace net {

TcpServer::TcpServer(EventLoop *loop):
		loop_(loop),
		bind_socket_(new TcpSocket()),
		bind_channel_(new Channel(loop, bind_socket_->fd())) {

	using namespace std::placeholders;
	bind_channel_->set_read_callback(std::bind(&TcpServer::on_new_connection, this, _1));
}

TcpServer::~TcpServer() {
	delete bind_channel_;
	delete bind_socket_;
	for (auto it = connections_.begin(); it != connections_.end(); ++it) {
		delete it->second;
	}
}

void TcpServer::start(const InetAddress &bind_addr) {
	bind_socket_->bind(bind_addr);
	bind_socket_->listen(5);
}

void TcpServer::on_new_connection(int fd) {
	assert(fd == bind_socket_->fd());
	InetAddress client_addr;
	int client = bind_socket_->accept(&client_addr);
	cout << client_addr.host() << ":" << client_addr.port() << " connected\n";	
	if (conn_cb_) {
		assert(connections_.find(client) == connections_.end());
		TcpConnection *conn = new TcpConnection(loop_, client);
		conn->set_message_callback(msg_cb_);
		conn->set_write_complete_callback(write_complete_cb_);
		// TODO error callback
		connections_[client] = conn;
		conn_cb_(conn);
	} else {
		::close(fd);
	}
}

} // namespace net
} // namespace sduzh


