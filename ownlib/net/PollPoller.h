#ifndef SDUZH_OWNLIB_NET_POLL_POLLER_H
#define SDUZH_OWNLIB_NET_POLL_POLLER_H

#include <vector>
#include <map>
#include <poll.h>
#include <ownlib/net/Poller.h>

namespace sduzh {
namespace net {

class PollPoller : public Poller {
public:
	PollPoller(): Poller(), fdindex_(), pollfds_() {
	}

	void update_event(int fd, int events) OVERRIDE;
	void remove_event(int fd) OVERRIDE;

	int poll(EventList *events, int timeout_ms=-1) OVERRIDE;

private:
	/// fd index in pollfd list, -1 if not exists
	int pollfd_index(int fd);

	typedef std::vector<struct pollfd> PollFdList;
	typedef std::map<int, int>         PollFdMap;

	PollFdMap  fdindex_;
	PollFdList pollfds_;
};

} // namespace net
} // namespace sduzh

#endif
