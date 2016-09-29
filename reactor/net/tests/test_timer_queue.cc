#include <reactor/base/DateTime.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TimerId.h>
#include <reactor/net/TimerQueue.h>

using namespace sduzh;
using namespace sduzh::base;
using namespace sduzh::net;

EventLoop loop;

void bar() {
	static int cnt = 0;
	LOG(Info) << "bar";
	if (++cnt == 10)
		loop.quit();
}

void foo() {
	LOG(Info) << "foo";
}

int main() {
	SimpleLogger::setLogLevel(Info);

	TimerQueue queue(&loop);

	DateTime now(DateTime::current());
	queue.add_timer(now + DateTime(0,0), bar, 1);
	queue.add_timer(now + DateTime(0,0), foo, 2);
	
	loop.loop();
	return 0;
}
