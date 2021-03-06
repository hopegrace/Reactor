#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>

using namespace reactor;
using namespace reactor::base;
using namespace reactor::net;

void f1() {
	 LOG(Info) << "f1";
}

void f2() {
	LOG(Info) << "f2";
}

void f3() {
	LOG(Info) << "f3";
}

int main() {
	EventLoop loop;

	loop.run_after(1, f1);
	loop.run_at(Timestamp::current().add_seconds(2), f2);
	loop.run_every(2, f3);

	loop.loop();
}
