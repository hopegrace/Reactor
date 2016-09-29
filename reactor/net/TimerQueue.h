#ifndef SDUZH_REACTOR_NET_TIMER_QUEUE_H
#define SDUZH_REACTOR_NET_TIMER_QUEUE_H

#include <utility>
#include <memory>
#include <set>
#include <vector>

#include <reactor/base/DateTime.h>
#include <reactor/net/Callbacks.h>
#include <reactor/net/Channel.h>

namespace sduzh {
namespace net {

class EventLoop;
class Timer;
class TimerId;

class TimerQueue {
public:
	TimerQueue(EventLoop *loop);
	~TimerQueue();

	/// disable copy-ctor, copy assignment
	TimerQueue(const TimerQueue &) = delete;
	TimerQueue & operator = (const TimerQueue &) = delete;

	/// repeat if @c interval > 0.0
	TimerId add_timer(const DateTime &when, const TimerCallback &cb, double interval);
	void cancel(TimerId id);

private:
	typedef std::pair<DateTime, Timer *> Entry;
	typedef std::set<Entry> TimerList;
	typedef std::set<Timer *> ExpiredList;

	/// channel callback
	void on_read();

	TimerId add_timer(Timer *timer);
	void earliest_changed();
	ExpiredList expired_timers();
	bool insert(Timer *timer);

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
