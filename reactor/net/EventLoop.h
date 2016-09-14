#ifndef SDUZH_OWNLIB_NET_EVENT_LOOP_H
#define SDUZH_OWNLIB_NET_EVENT_LOOP_H

#include <vector>

namespace sduzh {
namespace net {

class Channel;
class Poller;

class EventLoop {
public:
	EventLoop();
	~EventLoop();

	EventLoop(const EventLoop &) = delete;
	EventLoop& operator=(const EventLoop &) = delete;	

	void loop();

	void add_channel(Channel *channel);
	void remove_channel(Channel *channel);
	void update_channel(Channel *channel);

private:
	typedef std::vector<Channel*> ChannelList;

	Poller *poller_;
};

} // namespace net
} // namespace sduzh

#endif
