#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <reactor/net/Connector.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/InetAddress.h>

using namespace sduzh::net;

EventLoop loop;

void on_connected(int fd) {
	char buf[1024];
	std::cout << fd << " connected\n";
	::write(fd, "hello\n", 6);
	ssize_t nread = ::read(fd, buf, sizeof buf);
	assert(nread >= 0);
	buf[nread] = 0;
	std::cout << buf;
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
