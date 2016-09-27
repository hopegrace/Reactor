#include <reactor/base/DateTime.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TimerId.h>
#include <reactor/net/TimerQueue.h>

void foo() {
}

using namespace sduzh::base;
using namespace sduzh::net;

int main() {
	EventLoop loop;
	TimerQueue queue(&loop);

	queue.add_timer(DateTime::current() + DateTime(5,0), foo, 5);
	
	loop.loop();
	return 0;
}
