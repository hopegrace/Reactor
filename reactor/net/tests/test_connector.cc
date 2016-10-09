#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include <reactor/net/Connector.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/InetAddress.h>

using namespace reactor::net;

EventLoop loop;

void on_connected(int fd) {
	std::cout << fd << " connected\n";
	::close(fd);
	loop.quit();
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << basename(argv[0]) << " host port\n";
		return 0;
	}

	int port = atoi(argv[2]);

	Connector connector(&loop, InetAddress(argv[0], static_cast<uint16_t>(port)));
	connector.set_connection_callback(on_connected);
	connector.start();
	loop.loop();
	return 0;
}
