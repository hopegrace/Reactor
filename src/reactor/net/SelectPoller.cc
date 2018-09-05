#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <map>

#include <reactor/net/SelectPoller.h>
#include <reactor/net/Channel.h>

namespace reactor {
namespace net {

SelectPoller::SelectPoller(): Poller(), maxfd_(-1) {
	FD_ZERO(&readfds_);
	FD_ZERO(&writefds_);
	FD_ZERO(&exceptfds_);
}

void SelectPoller::update_channel(Channel *channel) {
	int fd = channel->fd();
	short events = channel->events();

	FD_CLR(fd, &readfds_);
	FD_CLR(fd, &writefds_);
	FD_CLR(fd, &exceptfds_);

	if (events & EVENT_READ)
		FD_SET(fd, &readfds_);
	if (events & EVENT_WRITE) 
		FD_SET(fd, &writefds_);
	if (events & EVENT_ERROR)
		FD_SET(fd, &exceptfds_);

	if (channels_.find(fd) == channels_.end())  {
		channels_[fd] = channel;
		if (fd > maxfd_) maxfd_ = fd;
	}
}

void SelectPoller::remove_channel(Channel *channel) {
	int fd = channel->fd();
	auto it = channels_.find(fd);
	if (it != channels_.end()) {
		channels_.erase(it);

		FD_CLR(fd, &readfds_);
		FD_CLR(fd, &writefds_);
		FD_CLR(fd, &exceptfds_);

		if (fd == maxfd_) update_maxfd();
	}
}

int SelectPoller::poll(ChannelList *active_channels, int timeout_ms) {
	int num_event = 0;

	fd_set readfds = readfds_;
	fd_set writefds = writefds_;
	fd_set exceptfds = exceptfds_;

	active_channels->clear();

	if (timeout_ms < 0) {
		num_event = ::select(1 + maxfd_, &readfds, &writefds, &exceptfds, 0); 
	} else {
		struct timeval tv;
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = timeout_ms % 1000;
		num_event = ::select(1 + maxfd_, &readfds, &writefds, &exceptfds, &tv);
	}
	assert(num_event <= static_cast<int>(channels_.size()));
	int savederrno = errno;
	if (num_event > 0 && active_channels) {
		for (int fd = 0; fd <= maxfd_; fd++) {
			short revents = EVENT_NONE;
			if (FD_ISSET(fd, &readfds)) {
				revents |= EVENT_READ;
			}
			if (FD_ISSET(fd, &writefds)) {
				revents |= EVENT_WRITE;
			}
			if (FD_ISSET(fd, &exceptfds)) {
				revents |= EVENT_ERROR;
			}

			if (revents != EVENT_NONE) {
				Channel *channel = channels_[fd];
				channel->set_revents(revents);
				active_channels->push_back(channel);
				if (static_cast<int>(active_channels->size()) == num_event)
					break;
			}
		}
	} else if (num_event < 0) {
		if (savederrno != EINTR) {
			errno = savederrno;
			perror("SelectPoller::select");
		}
	} 	
	return num_event;
}

void SelectPoller::update_maxfd() {
	for (int fd = maxfd_; fd >= 0; fd--) {
		if (FD_ISSET(fd, &readfds_) || FD_ISSET(fd, &writefds_) || 
				FD_ISSET(fd, &exceptfds_)) {
			maxfd_ = fd;
			assert(channels_.find(fd) != channels_.end());
			return;
		}
	}
	maxfd_ = -1;
}

} // namespace net
} // namespace reactor


