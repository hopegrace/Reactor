#ifndef SDUZH_OWNLIB_NET_POLLER_H
#define SDUZH_OWNLIB_NET_POLLER_H

#include <vector>
#include <ownlib/net/types.h>

namespace sduzh {
namespace net {

class Poller {
public:
	typedef std::vector<PollEvent> EventList;

	Poller() {}
	virtual ~Poller() {}

	virtual void update_event(int fd, int events) = 0;
	virtual void remove_event(int fd) = 0;

	/// -1 means infinity
	virtual int poll(EventList *events, int timeout_ms=-1) = 0;
};

} // namespace net
} // namespace sduzh

#endif
