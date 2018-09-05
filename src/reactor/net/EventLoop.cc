#include <reactor/net/EventLoop.h>

#include <signal.h>

#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Channel.h>
#include <reactor/net/PollPoller.h>
#include <reactor/net/TimerId.h>
#include <reactor/net/TimerQueue.h>

namespace reactor {
namespace net {

namespace {
#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
    // LOG_TRACE << "Ignore SIGPIPE";
  }
};
#pragma GCC diagnostic error "-Wold-style-cast"

IgnoreSigPipe initObj;
}

EventLoop::EventLoop(): 
	poller_(new PollPoller()), 
	timerq_(new TimerQueue(this)),
	quit_(false),
	handling_(false), 
	qevents_() {
}

EventLoop::~EventLoop() {
}

void EventLoop::loop() {
	while (!quit_) {
		poller_->poll(&active_channels_);

		handle_active_channels();
		handle_queue_events();
	}
}

void EventLoop::handle_active_channels() {
	handling_ = true;
	for (Channel *channel : active_channels_) {
		channel->handle_events();
	}
	handling_ = false;
}

void EventLoop::handle_queue_events() {
	assert(!handling_);
	while (!qevents_.empty()) {
		auto event = qevents_.front();
		qevents_.pop();
		event();
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
} // namespace reactor


