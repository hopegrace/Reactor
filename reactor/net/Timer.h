#ifndef SDUZH_REACTOR_NET_TIMER_H
#define SDUZH_REACTOR_NET_TIMER_H

#include <functional>
#include <reactor/base/DateTime.h>

namespace sduzh {
namespace net {

class TimerQueue;

/// internal class for TimerQueue use
class Timer {
public:
	typedef std::function<void()> Callback;

	Timer(const DateTime &time, const Callback &callback, double interval):
		expire_time_(time),
		callback_(callback),
		interval_(interval),
		repeat_(interval > 0.0) {

	}

	void expired() {
		callback_();

		if (repeat_) {
			size_t s = static_cast<size_t>(interval_);
			size_t ms = static_cast<size_t>(1000 * (interval_ - static_cast<double>(s)));
			expire_time_ += DateTime(s, static_cast<int>(ms));
		}
	}

	DateTime expire_time() const { return expire_time_; }
	bool repeat() const { return repeat_; }

private:
	DateTime expire_time_;
	Callback callback_;
	double interval_;
	bool repeat_;
};

} // namespace net
} // namespace sduzh

#endif
