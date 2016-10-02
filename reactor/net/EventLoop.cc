#include <reactor/net/EventLoop.h>

#include <reactor/net/Channel.h>
#include <reactor/net/PollPoller.h>
#include <reactor/net/TimerId.h>
#include <reactor/net/TimerQueue.h>

namespace sduzh {
namespace net {

EventLoop::EventLoop(): 
	poller_(new PollPoller()), 
	timerq_(new TimerQueue(this)),
	quit_(false) {
}

EventLoop::~EventLoop() {
}

void EventLoop::loop() {
	while (!quit_) {
		ChannelList active_channels;

		poller_->poll(&active_channels);

		for (Channel *channel : active_channels) {
			channel->handle_events();
		}
	}
}

void EventLoop::add_channel(Channel *channel) {
	poller_->update_channel(channel);
}

void EventLoop::remove_channel(Channel *channel) {
	poller_->remove_channel(channel);
}

void EventLoop::update_channel(Channel *channel) {
	poller_->update_channel(channel);
}

void EventLoop::cancel_timer(const TimerId &id) {
	timerq_->cancel(id);
}

TimerId EventLoop::run_after(double delay, const TimerCallback &cb) {
	int64_t delay_us = static_cast<int64_t>(delay * 1000000);
	return run_at(Timestamp::current().add_microseconds(delay_us), cb);
}

TimerId EventLoop::run_at(const Timestamp &time, const TimerCallback &cb) {
	return timerq_->add_timer(time, cb, 0.0);
}

TimerId EventLoop::run_every(double interval, const TimerCallback &cb) {
	int64_t delay_us = static_cast<int64_t>(interval * 1000000);
	Timestamp first(Timestamp::current().add_microseconds(delay_us));
	return timerq_->add_timer(first, cb, interval);
}

} // namespace net
} // namespace sduzh


