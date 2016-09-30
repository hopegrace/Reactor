#include <reactor/net/Timer.h>

namespace sduzh {
namespace net {

Timer::Timer(const DateTime &time, const TimerCallback &callback, double interval):
	when_(time),
	callback_(callback),
	interval_(interval),
	repeat_(interval > 0.0),
	cancled_(false) {

}

void Timer::cancel() {
	cancled_ = true;
	interval_ = 0.0;
	repeat_ = false;
}

void Timer::restart(const DateTime &now) {
	if (repeat_) {
		when_ = now.add_seconds(interval_);
	}
}

void Timer::run() {
	if (!cancled_) { callback_(); }
}

} // namespace net
} // namespace sduzh
