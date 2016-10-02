#ifndef SDUZH_OWNLIB_NET_EVENT_LOOP_H
#define SDUZH_OWNLIB_NET_EVENT_LOOP_H

#include <functional>
#include <memory>
#include <vector>

#include <reactor/net/Callbacks.h>
#include <reactor/base/Timestamp.h>
#include <reactor/net/TimerId.h>

namespace sduzh {
namespace net {

class Channel;
class Poller;
class TimerQueue;

class EventLoop {
public:
	EventLoop();
	~EventLoop();

	EventLoop(const EventLoop &) = delete;
	EventLoop& operator=(const EventLoop &) = delete;	

	void loop();

	void quit() { quit_ = true; }

	void cancel_timer(const TimerId &id);

	///
	/// run callback fater @c delay seconds
	///
	TimerId run_after(double delay, const TimerCallback &cb);
	///
	/// run callback at 'time'
	///
	TimerId run_at(const Timestamp &time, const TimerCallback &cb);
	///
	/// run callback every  @c interval seconds
	///
	TimerId run_every(double interval , const TimerCallback &cb);

	void add_channel(Channel *channel);
	void remove_channel(Channel *channel);
	void update_channel(Channel *channel);

private:
	typedef std::unique_ptr<Poller> PollerPtr;
	typedef std::unique_ptr<TimerQueue> TimerQueuePtr;
	typedef std::vector<Channel*> ChannelList;

	PollerPtr poller_;
	TimerQueuePtr timerq_;
	bool quit_;
};

} // namespace net
} // namespace sduzh

#endif
