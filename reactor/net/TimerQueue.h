#ifndef SDUZH_REACTOR_NET_TIMER_QUEUE_H
#define SDUZH_REACTOR_NET_TIMER_QUEUE_H

#include <utility>
#include <set>
#include <memory>
#include <reactor/base/DateTime.h>
#include <reactor/net/Channel.h>

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
	/// channel callback
	void on_read();

	typedef std::pair<DateTime, Timer*> Entry;
	typedef std::set<Entry> TimerList;

	EventLoop *loop_;
	int timerfd_;
	Channel channel_;

	TimerList timers_;
};

} // namespace net
} // namespace sduzh

#endif 
