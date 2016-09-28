#include <reactor/net/TimerQueue.h>

#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <reactor/base/DateTime.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Timer.h>
#include <reactor/net/TimerId.h>

using namespace std;
using namespace sduzh::base;

namespace sduzh {
namespace net {

namespace detail {

int create_timerfd_or_die() {
	int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
	if (fd < 0) {
		LOG(Fatal) << strerror(errno);
	}
	return fd;
}

} // namespace detail

TimerQueue::TimerQueue(EventLoop *loop):
	loop_(loop),
	timerfd_(detail::create_timerfd_or_die()),
	channel_(loop, timerfd_),
	timers_() {

    /// channel is enable reading by default
	channel_.set_read_callback(std::bind(&TimerQueue::on_read, this));
	channel_.enable_read();
}

TimerQueue::~TimerQueue() {
	channel_.disable_all();
	channel_.remove();
	::close(timerfd_);
}

TimerId TimerQueue::add_timer(const DateTime &when, const Callback &cb, double interval) {
	return add_timer(new Timer(when, cb, interval));
}

TimerId TimerQueue::add_timer(Timer *timer) {
	timers_.insert(std::make_pair(timer->when(), timer));
	auto it = timers_.begin();
	if (it->second == timer) {
		earliest_changed();
	}
	return TimerId();
}

void TimerQueue::earliest_changed() {
	struct itimerspec ts;

	if (!timers_.empty()) {
		Timer * timer = timers_.begin()->second;
		time_t when(timer->when().micro_seconds());
		ts.it_value.tv_sec = when / kMicroSecondsPerSecond;
		ts.it_value.tv_nsec = when % kMicroSecondsPerSecond * 1000; 
		LOG(Debug) << "earliest timer is " << timer->when().to_string();
	} else {
		ts.it_value.tv_sec = 0;
		ts.it_value.tv_nsec = 0;
		LOG(Debug) << "stop timer";
	}
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;

	int ret = timerfd_settime(timerfd_, TFD_TIMER_ABSTIME, &ts, NULL);
	if (ret < 0) {
		LOG(Error) << "timerfd_settime in TimerQueue::earliest_changed(): " << strerror(errno);
	}
}

TimerQueue::ExpiredList TimerQueue::expired_timers() {
	ExpiredList result;
	DateTime now(DateTime::current());
	auto end = timers_.upper_bound(std::make_pair(now, static_cast<Timer*>(nullptr)));
	for (auto it = timers_.begin(); it != end; ++it) {
		result.push_back(it->second);
	}
	timers_.erase(timers_.begin(), end);
	return result;
}

void TimerQueue::on_read() {
	errno = 0;
	uint64_t val;
	ssize_t ret = ::read(timerfd_, &val, sizeof val);
	if (ret != 8) {
		LOG(Error) << strerror(errno);
		return;
	}

	expires_ = expired_timers();
	for (Timer *t : expires_) {
		t->expired();
	}

	for (Timer *t : expires_) {
		if (t->repeat()) {
			add_timer(t);
		} else {
			delete t;
		}
	}

	expires_.clear();
}

} // namespace net
} // namespace sduzh
