#include <reactor/net/Timer.h>

namespace reactor {
namespace net {

Timer::Timer(const Timestamp &time, const TimerCallback &callback, double interval):
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

void Timer::restart(const Timestamp &now) {
	if (repeat_) {
		when_ = now + Timestamp(
				static_cast<int64_t>(interval_ * Timestamp::kMicroSecondsPerSecond));
	}
}

void Timer::run() {
	if (!cancled_) { callback_(); }
}

} // namespace net
} // namespace reactor
