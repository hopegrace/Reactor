#ifndef SDUZH_REACTOR_NET_TIMER_QUEUE_H
#define SDUZH_REACTOR_NET_TIMER_QUEUE_H

#include <utility>
#include <memory>
#include <set>
#include <vector>
#include <reactor/net/Channel.h>


namespace sduzh { 
namespace base { 
class DateTime;
}
}

using sduzh::base::DateTime;

namespace sduzh {
namespace net {

class EventLoop;
class Timer;
class TimerId;

class TimerQueue {
public:
	typedef std::function<void()> Callback;

	TimerQueue(EventLoop *loop);
	~TimerQueue();

	/// disable copy-ctor, copy assignment
	TimerQueue(const TimerQueue &) = delete;
	TimerQueue & operator = (const TimerQueue &) = delete;

	/// repeat if @c interval > 0.0
	TimerId add_timer(const DateTime &when, const Callback &cb, double interval);
	void cancel(TimerId id);

private:
	typedef std::pair<DateTime, Timer *> Entry;
	typedef std::set<Entry> TimerList;
	typedef std::vector<Timer *> ExpiredList;

	/// channel callback
	void on_read();

	TimerId add_timer(Timer *timer);
	void earliest_changed();
	ExpiredList expired_timers();

	const time_t kMicroSecondsPerSecond = 1000000;

	EventLoop *loop_;
	int timerfd_;
	Channel channel_;

	TimerList timers_;
	ExpiredList expires_;  // expired timers
};

} // namespace net
} // namespace sduzh

#endif 
