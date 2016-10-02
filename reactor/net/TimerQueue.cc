#include <reactor/net/TimerQueue.h>
#include <assert.h>
#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Timer.h>
#include <reactor/net/TimerId.h>

using namespace std;

namespace sduzh {
namespace net {

namespace detail {

int create_timerfd() {
	int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
	if (fd < 0) {
		LOG(Fatal) << strerror(errno);
	}
	return fd;
}

void set_timerfd(int fd, const Timestamp &when) {
	struct itimerspec ts;
	time_t us(when.microseconds());
	ts.it_value.tv_sec = us / Timestamp::kMicroSecondsPerSecond;
	ts.it_value.tv_nsec = us % Timestamp::kMicroSecondsPerSecond * 1000; 
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;

	int ret = timerfd_settime(fd, TFD_TIMER_ABSTIME, &ts, NULL);
	if (ret < 0) {
		LOG(Error) << "timerfd_settime(): " << strerror(errno);
	}
}

void close_timerfd(int fd) {
	set_timerfd(fd, Timestamp(0));
}

void read_timerfd(int fd) {
	uint64_t val;
	if (::read(fd, &val, sizeof val) != sizeof(val)) {
		LOG(Error) << "read_timerfd(): " << strerror(errno);
	}
}

} // namespace detail

TimerQueue::TimerQueue(EventLoop *loop):
	loop_(loop),
	timerfd_(detail::create_timerfd()),
	channel_(loop, timerfd_),
	timers_() {

    /// channel is enable reading by default
	channel_.set_read_callback(std::bind(&TimerQueue::on_read, this));
	channel_.enable_read();
}

TimerQueue::~TimerQueue() {
	/// no need to remove channel, because we are in dtor of EventLoop
	::close(timerfd_);
}

TimerId TimerQueue::add_timer(const Timestamp &when, const TimerCallback &cb, double interval) {
	return add_timer(std::make_shared<Timer>(when, cb, interval));
}

TimerId TimerQueue::add_timer(const TimerPtr &timer) {
	bool earliest_changed = insert(timer);
	if (earliest_changed) {
		reset();
	}
	return TimerId(timer);
}

void TimerQueue::cancel(const TimerId &id) {
	TimerPtr timer(id.lock());
	if (timer) { timer->cancel(); }
}

void TimerQueue::reset() {
	if (!timers_.empty()) {
		TimerPtr timer = timers_.begin()->second;
		detail::set_timerfd(timerfd_, timer->when());
		LOG(Debug) << "earliest time of timer queue is " << timer->when().to_string();
	} else {
		LOG(Debug) << "stop timerfd";
		detail::close_timerfd(timerfd_);
	}
}

TimerQueue::ExpireList TimerQueue::get_expired(const Timestamp &now) {
	ExpireList result;
	auto end = timers_.upper_bound(std::make_pair(now, TimerPtr()));
	for (auto it = timers_.begin(); it != end; ++it) {
		result.insert(it->second);
	}
	timers_.erase(timers_.begin(), end);

	return result;
}

bool TimerQueue::insert(const TimerPtr &timer) {
	auto ret = timers_.insert(std::make_pair(timer->when(), timer));
	assert(ret.second == true);
	return ret.first == timers_.begin(); // earliest changed
}

void TimerQueue::on_read() {
	Timestamp now(Timestamp::current());
	detail::read_timerfd(timerfd_);

	ExpireList expires = get_expired(now);
	for (TimerPtr timer: expires) {
		timer->run();
	}

	restart(expires, now);
}

void TimerQueue::restart(const ExpireList &expires, const Timestamp &now) {
	for (auto it = expires.begin(); it != expires.end(); ++it) {
		assert(it->use_count() == 1);
		if ((*it)->repeat()) {
			(*it)->restart(now);
			insert(*it);
		} 
	}

	reset();
}

} // namespace net
} // namespace sduzh
