#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <ownlib/net/PollPoller.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <algorithm>

namespace sduzh {
namespace net {

void PollPoller::update_event(int fd, int events) {
	struct pollfd pollfd;

	pollfd.fd = fd;
	pollfd.events = 0;
	pollfd.revents = 0;

	if (events & EVENT_READABLE)
		pollfd.events |= POLLIN | POLLRDHUP;
	if (events & EVENT_WRITABLE)
		pollfd.events |= POLLOUT;
	if (events & EVENT_ERROR)
		pollfd.events |= POLLPRI;

	int idx = pollfd_index(fd);
	if (idx == -1) {  // add
		pollfds_.push_back(pollfd);
		fdindex_[fd] = static_cast<int>(pollfds_.size()) - 1;
	} else { // update
		assert(fd == pollfds_[idx].fd);
		pollfds_[idx] = pollfd;
	}
}

void PollPoller::remove_event(int fd) {
	int idx = pollfd_index(fd);
	int last = static_cast<int>(pollfds_.size()) - 1;
	if (idx < 0 || last < 0) 
		return;

	if (idx != last) {
		std::swap(pollfds_[last], pollfds_[idx]);
		fdindex_[pollfds_[idx].fd] = idx;
		// fdindex_[fd] = last;
	}
	
	fdindex_.erase(fd);
	pollfds_.pop_back();
}

int PollPoller::poll(EventList *events, int timeout_ms) {
	int num_event = 0;
	num_event = ::poll(&pollfds_[0], static_cast<nfds_t>(pollfds_.size()), timeout_ms);
	events->clear();

	if (num_event < 0) {
		int save_errno = errno;
		if (errno != EINTR) {
			perror("PollPoller::poll");
		}
		errno = save_errno;
	} else if (num_event == 0) {
		/**/
	} else {
		for (int i=0, sz=static_cast<int>(pollfds_.size()); i < sz; i++) {
			struct pollfd pollfd = pollfds_[i];
			if (pollfd.revents != 0) {
				PollEvent poll_event;
				poll_event.fd = pollfd.fd;
				poll_event.events = EVENT_NONE;
				if (pollfd.revents & (POLLIN | POLLPRI | POLLRDHUP | POLLHUP)) 
					poll_event.events |= EVENT_READABLE;
				if (pollfd.revents & POLLOUT)
					poll_event.events |= EVENT_WRITABLE;
				if (pollfd.revents & (POLLERR | POLLNVAL))
					poll_event.events |= EVENT_ERROR;

				assert(poll_event.events != EVENT_NONE);
				events->push_back(poll_event);
				if (static_cast<int>(events->size()) == num_event)
					break;
			}
		}
	}
	return num_event;
}

int PollPoller::pollfd_index(int fd) {
	PollFdMap::iterator it = fdindex_.find(fd);
	if (it == fdindex_.end()) 
		return -1;
	return it->second;
}


} // namespace net
} // namespace sduzh
