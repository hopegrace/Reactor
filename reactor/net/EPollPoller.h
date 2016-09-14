#ifndef SDUZH_OWNLIB_NET_EPOLL_POLLER_H
#define SDUZH_OWNLIB_NET_EPOLL_POLLER_H

#include <unordered_map>
#include <sys/epoll.h>
#include <reactor/net/Poller.h>

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
	uint32_t to_epoll_events(int events);

	typedef std::unordered_map<int, Channel*> ChannelMap;

	int epfd_;	
	ChannelMap channels_;
};


} // namespace net
} // namespace sduzh

#endif
