#ifndef SDUZH_OWN_LIB_NET_SELECT_POLLER_H
#define SDUZH_OWN_LIB_NET_SELECT_POLLER_H

#include <vector>
#include <sys/select.h>

#include <ownlib/net/types.h>  // EVENT_NONE
#include <ownlib/net/Poller.h>

namespace sduzh {
namespace net {

class SelectPoller : public Poller {
public:
	SelectPoller();

	void update_event(int fd, int events) OVERRIDE;
	void remove_event(int fd) OVERRIDE;
	
	/// timeout < 0 means infinite
	int poll(EventList *events, int timeout_ms=-1) OVERRIDE;

private:
	void update_maxfd();

	int maxfd_;

	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;
}; 

} // namespace net
} // namespace sduzh


#endif
