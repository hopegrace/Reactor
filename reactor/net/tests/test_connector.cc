#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <reactor/net/Connector.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/InetAddress.h>

using namespace sduzh::net;

EventLoop loop;

void on_connected(int fd) {
	std::cout << fd << " connected\n";
	::close(fd);
	loop.quit();
}

int main()
{
	Connector connector(&loop, InetAddress("127.0.0.1", 9090));
	connector.set_connection_callback(on_connected);
	connector.start();
	loop.loop();
	return 0;
}
