#include <reactor/base/Timestamp.h>
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
TimerId id4;

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
	Timestamp now(Timestamp::current());
	id1 = queue.add_timer(now.add_seconds(1), bar, 1); 
	id2 = queue.add_timer(now.add_seconds(2), foo, 2); 
	id3 = queue.add_timer(now.add_seconds(21), cancel, 0);
	id4 = queue.add_timer(now.add_seconds(22), quit, 0);
	
	loop.loop();
	return 0;
}
