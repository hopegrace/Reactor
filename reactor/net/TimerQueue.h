#ifndef SDUZH_REACTOR_NET_TIMER_QUEUE_H
#define SDUZH_REACTOR_NET_TIMER_QUEUE_H

#include <utility>
#include <memory>
#include <set>
#include <vector>

#include <reactor/base/Timestamp.h>
#include <reactor/net/Callbacks.h>
#include <reactor/net/Channel.h>
#include <reactor/net/TimerId.h>

namespace sduzh {
namespace net {

class EventLoop;
class Timer;

class TimerQueue {
public:
	TimerQueue(EventLoop *loop);
	~TimerQueue();

	/// disable copy-ctor, copy assignment
	TimerQueue(const TimerQueue &) = delete;
	TimerQueue & operator = (const TimerQueue &) = delete;

	/// repeat if @c interval > 0.0
	TimerId add_timer(const Timestamp &when, const TimerCallback &cb, double interval);
	void cancel(const TimerId &id);

private:
	typedef std::shared_ptr<Timer> TimerPtr;
	typedef std::pair<Timestamp, TimerPtr> Entry;
	typedef std::set<Entry> TimerList;
	typedef std::set<TimerPtr> ExpireList;

	TimerId add_timer(const TimerPtr &);
	bool insert(const TimerPtr &);

	void on_read();
	void reset();

	ExpireList get_expired(const Timestamp &);
	void restart(const ExpireList &, const Timestamp &);

	const time_t kMicroSecondsPerSecond = 1000000;

	EventLoop *loop_;
	int timerfd_;
	Channel channel_;

	TimerList timers_;
};

} // namespace net
} // namespace sduzh

#endif 
