#include <reactor/net/Channel.h>

#include <iostream>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>

using namespace reactor::base;

namespace reactor {
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
	events_(EVENT_NONE),
	revents_(EVENT_NONE),
	index_(-1),
	read_cb_(std::bind(default_read_callback, fd_)),
	write_cb_(std::bind(default_write_callback, fd_)),
	close_cb_(std::bind(default_close_callback, fd_)),
	error_cb_(std::bind(default_error_callback, fd_)) 
{
	//loop_->add_channel(this);
}

Channel::~Channel() {
	loop_->remove_channel(this);
}

void Channel::handle_events() {
	if (revents_ & EVENT_READ)  { read_cb_(); }
	if (revents_ & EVENT_WRITE) { write_cb_(); }
	if (revents_ & EVENT_ERROR) { error_cb_(); }
	if (revents_ & EVENT_CLOSE) { close_cb_(); }
}

void Channel::update_channel() {
	loop_->update_channel(this);
}

void Channel::remove() {
	loop_->remove_channel(this);
}

} // namespace net
} // namespace reactor
