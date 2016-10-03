#include <reactor/net/Acceptor.h>
#include <reactor/net/EventLoop.h>

using namespace sduzh::net;

int main() {
	EventLoop loop;
	Acceptor acceptor(&loop, InetAddress("0.0.0.0", 9090));
	acceptor.listen();
	loop.loop();
}
