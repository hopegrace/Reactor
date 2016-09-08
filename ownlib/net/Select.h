#ifndef SDUZH_NET_SELECT_H
#define SDUZH_NET_SELECT_H

#include <vector>
#include <sys/select.h>

#include <ownlib/net/types.h>  // EVENT_NONE

namespace sduzh {
namespace net {

struct SelectEvent {
	int fd;
	int events;

	SelectEvent(): fd(-1), events(EVENT_NONE) {}
};

class Select {
public:
	typedef std::vector<SelectEvent> EventList;

	Select();

	void update_event(int fd, int events);
	void remove_event(int fd);
	
	/// timeout < 0 means infinite
	int select(EventList *events, int timeout_ms=-1);

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
