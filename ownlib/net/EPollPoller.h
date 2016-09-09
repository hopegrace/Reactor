#ifndef SDUZH_OWNLIB_NET_EPOLL_POLLER_H
#define SDUZH_OWNLIB_NET_EPOLL_POLLER_H

#include <sys/epoll.h>
#include <ownlib/net/Poller.h>

namespace sduzh {
namespace net {

class EPollPoller: public Poller {
public:
	/// see epoll_create(2)
	EPollPoller(int size);
	~EPollPoller();

	void update_channel(Channel *channel) override;
	void remove_channel(Channel *channel) override;

	int poll(ChannelList *active_channels, int timeout_ms=-1) override;

private:
	typedef std::vector<struct epoll_event> EPollList;

	int epfd_;	
	EPollList events_;  // wait events
};


} // namespace net
} // namespace sduzh

#endif
