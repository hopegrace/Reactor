#include <stdio.h>
#include "Select.h"
#include <map>

namespace sduzh {
namespace net {

Select::Select(): maxfd_(-1) {
	FD_ZERO(&readfds_);
	FD_ZERO(&writefds_);
	FD_ZERO(&exceptfds_);
}

void Select::update_event(int fd, int events) {
	FD_CLR(fd, &readfds_);
	FD_CLR(fd, &writefds_);
	FD_CLR(fd, &exceptfds_);

	if (events & EVENT_READABLE)
		FD_SET(fd, &readfds_);
	if (events & EVENT_WRITABLE) 
		FD_SET(fd, &writefds_);
	if (events & EVENT_ERROR)
		FD_SET(fd, &exceptfds_);

	if (fd > maxfd_) maxfd_ = fd;
}

void Select::remove_event(int fd) {
	FD_CLR(fd, &readfds_);
	FD_CLR(fd, &writefds_);
	FD_CLR(fd, &exceptfds_);

	if (fd == maxfd_) update_maxfd();
}

int Select::select(EventList *events, int timeout_ms) {
	int ret = 0;

	fd_set readfds = readfds_;
	fd_set writefds = writefds_;
	fd_set exceptfds = exceptfds_;

	events->clear();

	if (timeout_ms < 0) {
		ret = ::select(1 + maxfd_, &readfds, &writefds, &exceptfds, 0); 
	} else {
		struct timeval tv;
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = timeout_ms % 1000;
		ret = ::select(1 + maxfd_, &readfds, &writefds, &exceptfds, &tv);
	}

	int num_event = ret;
	if (num_event > 0 && events) {
		std::map<int, SelectEvent> file_event;
		for (int fd = 0; fd <= maxfd_; fd++) {
			SelectEvent event = file_event[fd]; 
			event.fd = fd;

			if (FD_ISSET(fd, &readfds)) {
				event.events |= EVENT_READABLE;
			}
			if (FD_ISSET(fd, &writefds)) {
				event.events |= EVENT_WRITABLE;
			}
			if (FD_ISSET(fd, &exceptfds)) {
				event.events |= EVENT_ERROR;
			}

			if (event.events != EVENT_NONE) {
				events->push_back(event);
				if (static_cast<int>(events->size()) == num_event)
					break;
			}
		}
	} else if (num_event < 0) {
		perror("select");
	}
	return num_event;
}

void Select::update_maxfd() {
	for (int fd = maxfd_; fd >= 0; fd--) {
		if (FD_ISSET(fd, &readfds_) || FD_ISSET(fd, &writefds_) || 
				FD_ISSET(fd, &exceptfds_)) {
			maxfd_ = fd;
			return;
		}
	}
	maxfd_ = -1;
}

} // namespace net
} // namespace sduzh


