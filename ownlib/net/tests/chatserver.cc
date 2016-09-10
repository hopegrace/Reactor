#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <map>
#include <vector>
#include <libgen.h> // basename
#include <ownlib/base/DateTime.h>
#include <ownlib/net/Channel.h>
#include <ownlib/net/EventLoop.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/SelectPoller.h>
#include <ownlib/net/TcpSocket.h>

using namespace std;
using namespace sduzh::base;
using namespace sduzh::net;

class ChatServer {
public:
	ChatServer(EventLoop *loop): 
		loop_(loop), sock_(), channel_(loop, sock_.fd()) 
	{
		sock_.set_reuse_addr(true);
		sock_.set_blocking(false);
		using namespace std::placeholders;
		channel_.set_read_callback(std::bind(&ChatServer::on_connection, this, _1));
	}

	~ChatServer() {
		for (auto i: clients_) {
			delete channels_[i.first];
			delete clients_[i.first];
		}
	}

	void listen(int port) {
		sock_.bind(InetAddress("0.0.0.0", 9090));
		sock_.listen(5);
	}

	void on_connection(int fd) {
		assert(fd == sock_.fd());	
		DateTime now(DateTime::current());
		InetAddress client_addr;
		int clientfd  = sock_.accept(&client_addr);
		printf("[%s]%s:%d connected\n", now.to_string(), client_addr.host(), client_addr.port());

		assert(clients_.find(clientfd) == clients_.end());
		assert(channels_.find(clientfd) == channels_.end());

		TcpSocket *socket = new TcpSocket(clientfd);
		socket->set_blocking(false);
		socket->send("Welcode!\n");

		Channel *channel = new Channel(loop_, clientfd);
		using namespace std::placeholders;
		channel->set_read_callback(std::bind(&ChatServer::on_message, this, _1));

		clients_[clientfd] = socket;
		channels_[clientfd] = channel;
	}

	void on_message(int fd) {
		char buffer[1024];
		assert(clients_.find(fd) != clients_.end());
		assert(channels_.find(fd) != channels_.end());

		TcpSocket *client = clients_[fd];
		DateTime now = DateTime::current();
		InetAddress peer = client->getpeername();
		int head = snprintf(buffer, sizeof buffer, "[%s]%s:%d:\n", now.to_string(),
						peer.host(), peer.port());
		ssize_t nread = client->recv(buffer + head, sizeof(buffer) - head);
		if (nread <= 0) {
			printf("[%s]%s:%d disconnected\n", now.to_string(), peer.host(), peer.port());
			delete client;
			delete channels_[fd];
			clients_.erase(fd);
			channels_.erase(fd);
		} else {
			for (auto it = clients_.begin(); it != clients_.end(); ++it) {
				if (it->second == client)
					continue;
				it->second->sendall(buffer, head + nread);
			}
		}
	}

private:
	typedef std::map<int, Channel*> ChannelMap;
	typedef std::map<int, TcpSocket*>  SocketMap;

	EventLoop *loop_;
	TcpSocket sock_;
	Channel channel_;

	ChannelMap channels_;
	SocketMap  clients_;
};

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s listen port\n", basename(argv[0]));
		return 1;
	}

	uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
	EventLoop loop;
	ChatServer server(&loop);
	server.listen(port);
	loop.loop();

	return 0;
}
