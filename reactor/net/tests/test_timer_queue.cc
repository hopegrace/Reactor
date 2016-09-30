#include <reactor/base/DateTime.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TimerId.h>
#include <reactor/net/TimerQueue.h>

using namespace sduzh;
using namespace sduzh::base;
using namespace sduzh::net;

EventLoop loop;
TimerQueue queue(&loop);

TimerId id1;
TimerId id2;
TimerId id3;

void bar() {
	static int cnt = 0;
	LOG(Info) << "timer1 " << ++cnt;
	if (cnt == 10) {
		LOG(Debug) << "cancel timer 1";
		queue.cancel(id1);
	}
}

void foo() {
	static int cnt = 0;
	LOG(Info) << "timer2 " << ++cnt;
	if (cnt == 10) {
		LOG(Debug) << "cancel timer 2";
		queue.cancel(id2);
	}
}

void cancel() {
	LOG(Debug) << "cancel invalid timer id";
	queue.cancel(id1);
	queue.cancel(id2);
}

void quit() {
	loop.quit();
}

int main() {
	DateTime now(DateTime::current());
	id1 = queue.add_timer(now + DateTime(0,0), bar, 1); // 10s
	id2 = queue.add_timer(now + DateTime(0,0), foo, 2); // 20s
	queue.add_timer(now + DateTime(21, 0), cancel, 0);
	queue.add_timer(now + DateTime(22, 0), quit, 0);
	
	loop.loop();
	return 0;
}
