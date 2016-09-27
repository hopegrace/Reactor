#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <reactor/net/PollPoller.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <algorithm>

#include <reactor/net/Channel.h>

namespace sduzh {
namespace net {

PollPoller::PollPoller():
	Poller(),
	channels_(),
	pollfds_()
{
}

PollPoller::~PollPoller() {
}

void PollPoller::update_channel(Channel *channel) {
	int fd = channel->fd();
	short events = channel->events();

	struct pollfd pollfd;
	pollfd.fd = fd;
	pollfd.events = 0; 
	pollfd.revents = 0;

	if (events & EVENT_READ)
		pollfd.events |= POLLIN; // 不支持urgent data(POLLPRI)
	if (events & EVENT_WRITE)
		pollfd.events |= POLLOUT;
	if (events & EVENT_CLOSE)
		pollfd.events |= POLLRDHUP;

	auto it = channels_.find(fd);
	if (it == channels_.end()) {  // add
		assert(channel->index() < 0);
		channels_[fd] = channel;
		pollfds_.push_back(pollfd);
		channel->set_index(static_cast<int>(pollfds_.size() - 1));
	} else { // update
		int idx = channel->index();
		assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
		assert(pollfds_[idx].fd == fd);
		pollfds_[idx] = pollfd;
	}
}

void PollPoller::remove_channel(Channel *channel) {
	auto it = channels_.find(channel->fd());
	if (it != channels_.end()) {
		int idx = channel->index();
		assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));

		if (idx != static_cast<int>(pollfds_.size())- 1) {
			channels_[ pollfds_.back().fd ]->set_index(idx);
			std::swap(pollfds_.back(), pollfds_[idx]);
		}

		channel->set_index(-1);	
		channels_.erase(it);
		pollfds_.pop_back();
	}
}

int PollPoller::poll(ChannelList *active_channels, int timeout_ms) {
	nfds_t nfds = static_cast<nfds_t>(pollfds_.size());
	int num_event = ::poll(&pollfds_[0], nfds, timeout_ms);

	active_channels->clear();

	if (num_event < 0) {
		int save_errno = errno;
		if (errno != EINTR) {
			perror("PollPoller::poll");
		}
		errno = save_errno;
	} else if (num_event > 0) {
		for (int i=0, sz=static_cast<int>(pollfds_.size()); i < sz; i++) {
			struct pollfd pollfd = pollfds_[i];
			if (pollfd.fd >= 0 && pollfd.revents != 0) {
				Channel *channel = channels_[pollfd.fd];
				short revents = EVENT_NONE;
				if (pollfd.revents & (POLLRDHUP | POLLHUP)) { revents |= EVENT_CLOSE; }
				if (pollfd.revents & (POLLERR | POLLNVAL))  { revents |= EVENT_ERROR; }
				if (pollfd.revents & (POLLIN | POLLPRI))    { revents |= EVENT_READ; }
				if (pollfd.revents & POLLOUT)               { revents |= EVENT_WRITE; }

				if ((revents & EVENT_CLOSE) && (revents & EVENT_READ)) {
					revents &= ~EVENT_CLOSE;
				}
				channel->set_revents(revents);
				active_channels->push_back(channel);
				if (static_cast<int>(active_channels->size()) == num_event)
					break;
			}
		}
	}
	return num_event;
}

} // namespace net
} // namespace sduzh
