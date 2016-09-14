#ifndef SDUZH_OWNLIB_NET_POLL_POLLER_H
#define SDUZH_OWNLIB_NET_POLL_POLLER_H

#include <vector>
#include <unordered_map>
#include <poll.h>

#include <reactor/net/Poller.h>

namespace sduzh {
namespace net {

class PollPoller : public Poller {
public:
	PollPoller();
	~PollPoller();

	void update_channel(Channel *channel) override;
	void remove_channel(Channel *channel) override;

	int poll(ChannelList *active_channels, int timeout_ms=-1) override;

private:
	typedef std::unordered_map<int, Channel*> ChannelMap;
	typedef std::vector<struct pollfd> 		  PollFdList;

	ChannelMap channels_;
	PollFdList pollfds_;
};

} // namespace net
} // namespace sduzh

#endif
