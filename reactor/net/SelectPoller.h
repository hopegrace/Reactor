#ifndef SDUZH_OWN_LIB_NET_SELECT_POLLER_H
#define SDUZH_OWN_LIB_NET_SELECT_POLLER_H

#include <vector>
#include <unordered_map>
#include <sys/select.h>

#include <reactor/net/types.h>  // EVENT_NONE
#include <reactor/net/Poller.h>

namespace reactor {
namespace net {

class SelectPoller : public Poller {
public:
	SelectPoller();

	void update_channel(Channel *channel) override;
	void remove_channel(Channel *channel) override;
	
	/// timeout < 0 means infinite
	int poll(ChannelList *active_channels, int timeout_ms=-1) override;

private:
	void update_maxfd();

	typedef std::unordered_map<int, Channel*> ChannelMap;

	int maxfd_;

	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;

	ChannelMap channels_;
}; 

} // namespace net
} // namespace reactor


#endif
