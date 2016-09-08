#ifndef SDUZH_OWNLIB_NET_CHANNEL_H
#define SDUZH_WONLIB_NET_CHANNEL_H

namespace sduzh {
namespace net {

class Channel {
public:
	Channel(int fd): fd_(fd) {}

	short events() const { return events_; }

	void set_events(short events); 

private:
	int fd_;
	short events_;  // requested events
	short revents_; // returned events	
};

} // namespace net
} // namespace sduzh

#endif
