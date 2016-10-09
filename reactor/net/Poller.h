#ifndef SDUZH_OWNLIB_NET_POLLER_H
#define SDUZH_OWNLIB_NET_POLLER_H

#include <vector>
#include <reactor/net/types.h>

namespace reactor {
namespace net {

class Channel;

class Poller {
public:
	typedef std::vector<Channel*> ChannelList;

	Poller() {}
	virtual ~Poller() {}

	/// add or update a channel
	virtual void update_channel(Channel *channel) = 0;
	virtual void remove_channel(Channel *channel) = 0;

	/// -1 means infinity
	virtual int poll(ChannelList *active_channels, int timeout_ms=-1) = 0;
};

} // namespace net
} // namespace reactor

#endif
