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
	typedef std::vector<int> 	     FdSet;
	typedef std::vector<SelectEvent> EventList;

	Select();
	Select(const FdSet &rlist, const FdSet &wlist, const FdSet &xlist);
	
#if __cplusplus > 199711L
	Select(const Select &other) = default;
	Select &operator=(const Select &other) = default;
	~Select() = default;
#endif

	void add_exceptfd(int fd);
	void add_readfd(int fd);  
	void add_writefd(int fd); 

	void remove_exceptfd(int fd);
	void remove_readfd(int fd);  
	void remove_writefd(int fd); 

	void clear_exceptset();
	void clear_readset();  
	void clear_writeset(); 

	void set_exceptset(const FdSet &xlist);
	void set_readset(const FdSet &rlist);
	void set_writeset(const FdSet &wlist);

	/// timeout < 0 means infinite
	int select(EventList *events, int timeout_ms=-1);

private:
	void update_max_xfd();
	void update_max_rfd();
	void update_max_wfd();

	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;

	int max_xfd_;
	int max_rfd_;
	int max_wfd_;
}; 

} // namespace net
} // namespace sduzh


#endif
