#ifndef SDUZH_REACTOR_NET_TIMER_H
#define SDUZH_REACTOR_NET_TIMER_H

#include <functional>
#include <memory>
#include <reactor/base/DateTime.h>

namespace sduzh { 
namespace base {

class DateTime;

}}

using sduzh::base::DateTime;

namespace sduzh {
namespace net {

/// internal class for TimerQueue use
class Timer {
public:
	typedef std::function<void()> Callback;

	Timer(const DateTime &time, const Callback &callback, double interval):
		when_(time),
		callback_(callback),
		interval_(interval),
		repeat_(interval > 0.0) {

	}

	void cancel() {
		interval_ = 0.0;
		repeat_ = false;
	}

	void expired() {
		callback_();

		if (repeat_) {
			size_t s = static_cast<size_t>(interval_);
			size_t ms = static_cast<size_t>(1000 * (interval_ - static_cast<double>(s)));
			when_ += DateTime(s, static_cast<int>(ms));
		}
	}

	/// the next unhandled expired time
	DateTime when() const { return when_; }
	bool repeat() const { return repeat_; }

private:
	DateTime when_;  
	Callback callback_;
	double interval_;
	bool repeat_;
};

} // namespace net
} // namespace sduzh

#endif
