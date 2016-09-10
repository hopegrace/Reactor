#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <map>
#include <ownlib/net/Channel.h>
#include <ownlib/net/EventLoop.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/TcpSocket.h>

using namespace sduzh::net;

class EchoServer {
public:
	EchoServer(EventLoop *loop): 
		loop_(loop), 
		server_(), 
		channel_(loop, server_.fd()) 
	{
		assert(server_.fd() >= 0);

	}

	~EchoServer() {
		for (auto it = channels_.begin(); it != channels_.end(); ++it) {
			::close(it->first);
			delete it->second;
		}
	}

	void listen(uint16_t port) {
		server_.bind(InetAddress("0.0.0.0", static_cast<uint16_t>(port)));
		server_.listen(5);
	}

	void new_connection(int fd) {
		assert(server_.fd() == fd);

		InetAddress client_address;
		int clifd = server_.accept(&client_address);
		assert(clifd > 0);
		
		Channel *channel = new Channel(loop_, clifd);
		using namespace std::placeholders;
		channel->set_read_callback(std::bind(&EchoServer::new_message, this, _1));

		channels_[clifd] = channel;
	}

	void new_message(int fd) {
		
	}

private:
	EventLoop *loop_;

	typedef std::map<int, Channel*> ChannelMap;

	TcpSocket server_;
	Channel channel_;

	ChannelMap channels_;
};

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage; %s port", basename(argv[0]));
		return 1;
	}
	int port = atoi(argv[1]);
		
	return 0;
}
