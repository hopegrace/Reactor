#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <reactor/net/EventLoop.h>
#include <reactor/net/http/HTTPServer.h>

using namespace reactor::net;
using namespace reactor::net::http;

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << basename(argv[0]) << " host port\n";
		return 0;
	}

	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

	EventLoop loop;
	HTTPServer server(&loop, InetAddress(argv[1], port), 0); // TODO handler
	server.start();
	loop.loop();
	return 0;
}
