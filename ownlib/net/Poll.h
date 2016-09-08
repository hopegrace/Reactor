#ifndef SDUZH_OWNLIB_NET_POLL_H
#define SDUZH_OWNLIB_NET_POLL_H

#include <vector>
#include <poll.h>
#include <ownlib/net/types.h>

namespace sduzh {
namespace net {

class Poll {
public:
	typedef std::vector<PollEvent> EventList;

	Poll();
	
	void update_event(int fd, int events);
	void remove_event(int fd);

	int poll(EventList *events, int timeout_ms=-1);
	
private:
	std::vector<struct pollfd> fds_;
};

} //namespace net
} //namespace sduzh

#endif
