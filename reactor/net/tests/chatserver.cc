#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <map>
#include <vector>
#include <libgen.h> // basename
#include <reactor/base/Timestamp.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Channel.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpConnection.h>
#include <reactor/net/TcpServer.h>

using namespace std;
using namespace reactor::base;
using namespace reactor::net;

class ChatServer {
public:
	ChatServer(EventLoop *loop, InetAddress addr): 
		loop_(loop), 
		server_(loop, addr) {

		using namespace std::placeholders;
		server_.set_connection_callback(std::bind(&ChatServer::on_connection, this, _1));
		server_.set_message_callback(std::bind(&ChatServer::on_message, this, _1));
	}

	void start() {
		server_.start();
	}

private:
	void on_connection(const TcpConnectionPtr &conn) {
		InetAddress peer = conn->peer_address();
		if (conn->connected()) {
			LOG(Info) << peer.host() << ":" << static_cast<long>(peer.port()) << " connected";
			clients_[conn->fd()] = conn;
		} else {
			LOG(Info) << peer.host() << ":" << static_cast<long>(peer.port()) << " disconnected";
			clients_.erase(conn->fd());
		}
	}

	void on_message(const TcpConnectionPtr &conn) {
		InetAddress address = conn->peer_address();
		Buffer *msg = conn->message();
		for (auto it = clients_.begin(); it != clients_.end(); ++it) {
			TcpConnectionPtr &client = it->second;
			if (client != conn) {
				client->write(msg->data(), msg->readable_bytes());
			}
		}
		msg->clear();
	}

	typedef std::map<int, TcpConnectionPtr> ClientList;

	EventLoop *loop_;
	TcpServer server_;
	ClientList clients_;
};

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s listen port\n", basename(argv[0]));
		return 1;
	}

	EventLoop loop;
	ChatServer server(&loop, InetAddress("0.0.0.0", 9090));
	server.start();
	loop.loop();

	return 0;
}
