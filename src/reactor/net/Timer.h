#ifndef SDUZH_REACTOR_NET_TIMER_H
#define SDUZH_REACTOR_NET_TIMER_H

#include <functional>
#include <memory>

#include <reactor/base/Timestamp.h>
#include <reactor/net/Callbacks.h>

namespace reactor {
namespace net {

/// internal class for TimerQueue use
class Timer {
public:
	Timer(const Timestamp &time, const TimerCallback &callback, double interval);

	void cancel(); 
	void restart(const Timestamp &now);
	void run();

	Timestamp when() const { return when_; }
	bool repeat() const { return repeat_; }

private:
	Timestamp when_;  
	TimerCallback callback_;
	double interval_;
	bool repeat_;
	bool cancled_;
};

} // namespace net
} // namespace reactor

#endif
