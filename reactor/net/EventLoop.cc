#include <reactor/net/EventLoop.h>
#include <reactor/net/PollPoller.h>
#include <reactor/net/Channel.h>

namespace sduzh {
namespace net {

EventLoop::EventLoop(): poller_(new PollPoller()) {
}

EventLoop::~EventLoop() {
	delete poller_;
}

void EventLoop::loop() {
	for (;;) {
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

} // namespace net
} // namespace sduzh


