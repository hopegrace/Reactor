#include <ownlib/net/Channel.h>
#include <ownlib/net/EventLoop.h>

namespace sduzh {
namespace net {

Channel::Channel(EventLoop *loop, int fd):
	loop_(loop),
	fd_(fd),
	events_(EVENT_READABLE),
	revents_(EVENT_NONE),
	index_(-1),
	read_callback_(),
	write_callback_(),
	error_callback_() 
{
	loop_->add_channel(this);
}

Channel::~Channel() {
	events_ = EVENT_NONE;
	loop_->remove_channel(this);
}


void Channel::process_events() {
	if ((events_ & EVENT_READABLE) && (revents_ & EVENT_READABLE)) {
		if (read_callback_) {
			read_callback_(fd_);
		}
	}

	if ((events_ & EVENT_WRITABLE) && (revents_ & EVENT_WRITABLE)) {
		if (write_callback_) {
			write_callback_(fd_);
		}
	}

	if ((events_ & EVENT_ERROR) && (revents_ & EVENT_ERROR)) {
		if (error_callback_) {
			error_callback_(fd_);
		}
	}

	revents_ = EVENT_NONE;
}

void Channel::update_channel() {
	loop_->update_channel(this);
}

} // namespace net
} // namespace sduzh
