#include <reactor/net/EPollPoller.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Channel.h>

using namespace sduzh::base;

namespace sduzh {
namespace net {

EPollPoller::EPollPoller(int size): 
	Poller(),
	epfd_(::epoll_create(size)),
	channels_() {
}

EPollPoller::~EPollPoller() {
	::close(epfd_);
}

void EPollPoller::update_channel(Channel *channel) {
	auto it = channels_.find(channel->fd());
	struct epoll_event epoll_event;
	int ret = 0;

	epoll_event.data.fd = channel->fd();
	epoll_event.events = to_epoll_events(channel->events());

	if (it == channels_.end()) {
		channels_[channel->fd()] = channel;
		ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, channel->fd(), &epoll_event);
	} else {
		ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, channel->fd(), &epoll_event);
	}

	if (ret < 0) {
		LOG(Error) << strerror(errno);
	}
}

void EPollPoller::remove_channel(Channel *channel) {
	if (channels_.erase(channel->fd()) == 1) {
		struct epoll_event epoll_event;
		epoll_event.data.fd = channel->fd();
		epoll_event.events = 0;
		int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, channel->fd(), &epoll_event);
		if (ret < 0) {
			LOG(Error) << strerror(errno);
		}
	}
}

int EPollPoller::poll(ChannelList *active_channels, int timeout_ms) {
	std::vector<struct epoll_event> events(channels_.size());
	int nevent = epoll_wait(epfd_, &events[0], static_cast<int>(events.size()), timeout_ms);
	if (nevent < 0) {
		LOG(Error) << strerror(errno);
	} else if (nevent > 0) {
		for (int i = 0; i < nevent; i++) {
			int fd = events[i].data.fd;
			int revents = EVENT_NONE;
			Channel *channel = channels_[fd];
			if (events[i].events & EPOLLIN) { revents |= EVENT_READ; }
			if (events[i].events & EPOLLOUT) { revents |= EVENT_WRITE; }
			if (events[i].events & EPOLLRDHUP) { revents |= EVENT_CLOSE; }
			if (events[i].events & EPOLLHUP) { revents |= EVENT_CLOSE; }
			if (events[i].events & EPOLLERR) { revents |= EVENT_ERROR; }
			
			if ((revents & EVENT_CLOSE) && (revents & EVENT_READ)) {
				revents &= ~EVENT_CLOSE;
			}
			channel->set_revents(static_cast<short>(revents));
			active_channels->push_back(channel);
		}
	}
	return nevent;
}

uint32_t EPollPoller::to_epoll_events(int events) {
	uint32_t epoll_events = 0;
	if (events & EVENT_READ)  { epoll_events |= EPOLLIN; }
	if (events & EVENT_WRITE) { epoll_events |= EPOLLOUT; }
	if (events & EVENT_CLOSE) { epoll_events |= EPOLLRDHUP; }
	// epoll_wait will always wait for EPOLLERR, no need to set it
	// epoll_wait will always wait for EPOLLHUP, no need to set it
	return epoll_events;
}

} // namespace net
} // namespace sduzh
