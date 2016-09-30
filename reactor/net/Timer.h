#ifndef SDUZH_REACTOR_NET_TIMER_H
#define SDUZH_REACTOR_NET_TIMER_H

#include <functional>
#include <memory>

#include <reactor/base/DateTime.h>
#include <reactor/net/Callbacks.h>

namespace sduzh {
namespace net {

/// internal class for TimerQueue use
class Timer {
public:
	Timer(const DateTime &time, const TimerCallback &callback, double interval);

	void cancel(); 
	void restart(const DateTime &now);
	void run();

	DateTime when() const { return when_; }
	bool repeat() const { return repeat_; }

private:
	DateTime when_;  
	TimerCallback callback_;
	double interval_;
	bool repeat_;
	bool cancled_;
};

} // namespace net
} // namespace sduzh

#endif
