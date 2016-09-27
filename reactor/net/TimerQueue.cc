#include <reactor/net/TimerQueue.h>

#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>

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
	// TODO 
	struct itimerspec ts;
	ts.it_value.tv_sec = when.micro_seconds() / 1000000;
	ts.it_value.tv_nsec = (when.micro_seconds() % 1000000) * 1000; 

	time_t s = static_cast<time_t>(interval);
	ts.it_interval.tv_sec = s;
	ts.it_interval.tv_nsec = static_cast<long>((interval - static_cast<double>(s)) * 1000000000);  // 10^9
	int ret = timerfd_settime(timerfd_, TFD_TIMER_ABSTIME, &ts, NULL);
	if (ret < 0) {
		LOG(Error) << strerror(errno);
	}

	return TimerId();
}

void TimerQueue::on_read() {
	errno = 0;
	uint64_t val;
	ssize_t ret = ::read(timerfd_, &val, sizeof val);
	if (ret != 8) {
		LOG(Error) << strerror(errno);
	}
	// TODO call back
	LOG(Debug) << "timeout";
}

} // namespace net
} // namespace sduzh
