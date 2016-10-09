#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TcpClient.h>

using namespace reactor::net;

EventLoop loop;

void on_connection(const TcpConnectionPtr &conn) {
	if (conn->connected()) {
		printf("%s connected\n", conn->peer_address().to_string().c_str());
	} else {
		printf("%s disconnected\n", conn->peer_address().to_string().c_str());
		loop.quit();
	}
}

void on_message(const TcpConnectionPtr &conn) {
	Buffer *buffer = conn->message();
	printf("receive %lu bytes from server\n", buffer->readable_bytes());
	buffer->retrieve_all();
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s host, port\n", basename(argv[0]));
		return 0;
	}

	const char *host = argv[1];
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

	TcpClient client(&loop, InetAddress(host, port));
	client.set_connection_callback(on_connection);
	client.set_message_callback(on_message);
	client.connect();

	loop.loop();
}
