#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <libgen.h>
#include <unistd.h>

#include <reactor/net/EventLoop.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpClient.h>
#include <reactor/net/TcpConnection.h>

using namespace reactor::net;

class ChatClient {
public:
	ChatClient(EventLoop *loop, const InetAddress &server):
		loop_(loop),
		client_(loop, server),
		stdin_channel_(loop, STDIN_FILENO) {
		
		using namespace std::placeholders;
		client_.set_connection_callback(std::bind(&ChatClient::on_connection, this, _1));
		client_.set_message_callback(std::bind(&ChatClient::on_message, this, _1));

		stdin_channel_.set_read_callback(std::bind(&ChatClient::on_stdin_read, this));
		stdin_channel_.enable_read();
	}

	void start() {
		client_.connect();
	}

private:
	void on_connection(const TcpConnectionPtr &conn) {
		if (conn->connected()) {
			printf("[ connected to server ]\n");
		} else {
			// TODO restart
			loop_->quit();
		}
	}

	void on_message(const TcpConnectionPtr &conn) {
		Buffer *buffer = conn->buffer();
		// TODO Buffer::to_string() or Buffer::read_as_string() 
		printf("%.*s", static_cast<int>(buffer->readable_bytes()), buffer->data());
		buffer->retrieve_all();
	}

	void on_stdin_read() {
		char buffer[1024];
		ssize_t nread = ::read(STDIN_FILENO, buffer, sizeof(buffer)-1);
		if (nread > 0) {
			buffer[nread] = '\0';
			if (client_.connection()) {
				client_.connection()->write(buffer);
			}
		}
	}

	EventLoop *loop_;
	TcpClient client_;
	Channel stdin_channel_;
};

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <server host> <server port>\n", ::basename(argv[0]));
		return 0;
	}

	const char *server = argv[1];
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

	EventLoop loop;
	ChatClient client(&loop, InetAddress(server, port));
	client.start();
	loop.loop();

	return 0;
}
