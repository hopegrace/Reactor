#include <ownlib/net/EPollPoller.h>

#include <sys/epoll.h>
#include <unistd.h>

namespace sduzh {
namespace net {

EPollPoller::EPollPoller(int size): 
	Poller(),
	epfd_(::epoll_create(size)),
	events_() {
}

EPollPoller::~EPollPoller() {
	::close(epfd_);
}

void EPollPoller::update_channel(Channel *channel) {
	// TODO
}

void EPollPoller::remove_channel(Channel *channel) {
	// TODO
}

int EPollPoller::poll(ChannelList *active_channels, int timeout_ms) {
	// TODO
	return 0;
}

} // namespace net
} // namespace sduzh
