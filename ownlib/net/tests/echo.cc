#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <iostream>
#include <functional>
#include <map>
#include <ownlib/net/EventLoop.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/TcpConnection.h>
#include <ownlib/net/TcpServer.h>

using namespace sduzh::net;

class EchoServer {
public:
	EchoServer(EventLoop *loop): 
		loop_(loop), 
		server_(loop)
	{
		using namespace std::placeholders;
		server_.set_connection_callback(std::bind(&EchoServer::new_connection, this, _1));
		server_.set_message_callback(std::bind(&EchoServer::new_message, this, _1));
	}

	void start(uint16_t port) {
		server_.start(InetAddress("0.0.0.0", port));
	}

	void new_connection(const TcpConnectionPtr &conn) {
		if (conn->connected()) {
			clients_[conn->fd()] = conn;
		} else {
			clients_.erase(conn->fd());
		}
	}	

	void new_message(const TcpConnectionPtr &conn) {	
		conn->write(conn->message()->data(), conn->message()->size());
		conn->message()->clear();
	}

private:
	EventLoop *loop_;

	typedef std::map<int, TcpConnectionPtr> ClientMap;

	TcpServer server_;
	ClientMap clients_;
};

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage; %s port", basename(argv[0]));
		return 1;
	}
	int port = atoi(argv[1]);

	EventLoop loop;
	EchoServer server(&loop);
	server.start(static_cast<uint16_t>(port));
	loop.loop();
		
	return 0;
}
