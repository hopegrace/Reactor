#include <ownlib/net/Channel.h>

#include <iostream>
#include <ownlib/base/SimpleLogger.h>
#include <ownlib/net/EventLoop.h>

using namespace sduzh::base;

namespace sduzh {
namespace net {

void default_read_callback(int fd) {
	// TODO logging
	LOG(Debug) << "read event of fd " << fd;
}

void default_write_callback(int fd) {
	LOG(Debug) << "write evetn of fd " << fd;
}

void default_close_callback(int fd) {
	LOG(Debug) << "close event of fd " << fd;
}

void default_error_callback(int fd) {
	// TODO error message or error code
	LOG(Debug) << "error event of fd " << fd;
}

Channel::Channel(EventLoop *loop, int fd):
	loop_(loop),
	fd_(fd),
	events_(EVENT_READ),
	revents_(EVENT_NONE),
	index_(-1),
	read_cb_(default_read_callback),
	write_cb_(default_write_callback),
	close_cb_(default_close_callback),
	error_cb_(default_error_callback) 
{
	loop_->add_channel(this);
}

Channel::~Channel() {
	events_ = EVENT_NONE;
	loop_->remove_channel(this);
}

void Channel::handle_events() {
	if (revents_ & EVENT_READ) {
		read_cb_(fd_);
	}
	if (revents_ & EVENT_WRITE) {
		write_cb_(fd_);
	}
	if (revents_ & EVENT_ERROR) {
		error_cb_(fd_);
	}
	if (revents_ & EVENT_CLOSE) {
		close_cb_(fd_);
	}

	// revents_ = EVENT_NONE;
}

void Channel::update_channel() {
	loop_->update_channel(this);
}

} // namespace net
} // namespace sduzh
