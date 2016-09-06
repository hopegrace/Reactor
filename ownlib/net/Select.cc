#include <stdio.h>
#include "Select.h"
#include <map>

namespace sduzh {
namespace net {

namespace detail{

inline int max_int(int a, int b) {
	return a > b ? a : b;
}

inline int max_int_three(int a, int b, int c) {
	return max_int(a, max_int(b, c));
}

} 

Select::Select() {
	FD_ZERO(&readfds_);
	FD_ZERO(&writefds_);
	FD_ZERO(&exceptfds_);
	max_xfd_ = 0;
	max_rfd_ = 0;
	max_wfd_ = 0;
}

Select::Select(const FdSet &rlist, const FdSet &wlist, const FdSet &xlist) {
	set_readset(rlist);
	set_writeset(wlist);
	set_exceptset(xlist);
	max_xfd_ = 0;
	max_rfd_ = 0;
	max_wfd_ = 0;
}

void Select::add_exceptfd(int fd) { 
	FD_SET(fd, &exceptfds_); 
	if (fd > max_xfd_) max_xfd_ = fd;
}

void Select::add_readfd(int fd)   { 
	FD_SET(fd, &readfds_); 
	if (fd > max_rfd_) max_rfd_ = fd;
}

void Select::add_writefd(int fd)  { 
	FD_SET(fd, &writefds_); 
	if (fd > max_wfd_) max_wfd_ = fd;
}

void Select::remove_exceptfd(int fd) { 
	FD_CLR(fd, &exceptfds_); 
	update_max_xfd();
}

void Select::remove_readfd(int fd)   { 
	FD_CLR(fd, &readfds_); 
	update_max_rfd();
}

void Select::remove_writefd(int fd)  { 
	FD_CLR(fd, &writefds_); 
	update_max_wfd();
}

void Select::clear_exceptset() { 
	FD_ZERO(&exceptfds_); 
	max_xfd_ = -1;
}
void Select::clear_readset()   { 
	FD_ZERO(&readfds_); 
	max_rfd_ = -1;
}

void Select::clear_writeset()  { 
	FD_ZERO(&writefds_); 
	max_wfd_ = -1;
}

void Select::set_readset(const FdSet &rlist) {
	clear_readset();
	for (int i=0, sz=static_cast<int>(rlist.size()); i < sz; i++) {
		add_readfd(rlist[i]);
	}
}

void Select::set_writeset(const FdSet &wlist) {
	clear_writeset();
	for (int i=0, sz=static_cast<int>(wlist.size()); i < sz; i++) {
		add_writefd(wlist[i]);
	}
}

void Select::set_exceptset(const FdSet &xlist) {
	clear_exceptset();
	for (int i=0, sz=static_cast<int>(xlist.size()); i < sz; i++) {
		add_exceptfd(xlist[i]);
	}
}

int Select::select(EventList *events, int timeout_ms) {
	int ret = 0;
	int nfds = 1 + detail::max_int_three(max_xfd_, max_rfd_, max_wfd_);
	// copy fdset, may affect performance
	fd_set readfds = readfds_;
	fd_set writefds = writefds_;
	fd_set exceptfds = exceptfds_;

	events->clear();

	if (timeout_ms < 0) {
		ret = ::select(nfds, &readfds, &writefds, &exceptfds, 0); 
	} else {
		struct timeval tv;
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = timeout_ms % 1000;
		ret = ::select(nfds, &readfds, &writefds, &exceptfds, &tv);
	}

	int num_event = ret;
	if (num_event > 0 && events) {
		std::map<int, SelectEvent> file_event;
		for (int fd = 0; fd < nfds; fd++) {
			SelectEvent event = file_event[fd]; // may waste space
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
	} else if (num_event == 0) {
		printf("select timeout\n");
	} else {
		perror("select");
	}
	return num_event;
}

void Select::update_max_xfd() {
	int old = max_xfd_;
	max_xfd_ = -1;
	for (int fd = 0; fd <= old; fd++) {
		if (FD_ISSET(fd, &exceptfds_)) 
			max_xfd_ = fd;
	}
}

void Select::update_max_rfd() {
	int old = max_rfd_;
	max_rfd_ = -1;
	for (int fd = 0; fd <= old; fd++) {
		if (FD_ISSET(fd, &readfds_)) 
			max_rfd_ = fd;
	}
}

void Select::update_max_wfd() {
	int old = max_wfd_;
	max_wfd_ = -1;
	for (int fd = 0; fd <= old; fd++) {
		if (FD_ISSET(fd, &writefds_)) 
			max_wfd_ = fd;
	}
}

} // namespace net
} // namespace sduzh


