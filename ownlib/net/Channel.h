#ifndef SDUZH_OWNLIB_NET_CHANNEL_H
#define SDUZH_OWNLIB_NET_CHANNEL_H

#include <functional>

#include <ownlib/net/types.h>

namespace sduzh {
namespace net {

class EventLoop;

class Channel {
public:
	typedef std::function<void (int)> EventCallback;

	Channel(EventLoop *loop, int fd);
	~Channel();
	
	Channel(const Channel &) = delete;
	Channel &operator=(const Channel &) = delete;

	int fd() const { return fd_; }

	void handle_events();

	void disable_read()  { events_ &= ~(EVENT_READ | EVENT_CLOSE); update_channel(); }
	void disable_write() { events_ &= ~EVENT_WRITE; update_channel(); }
	void disable_all()   { events_ = EVENT_NONE; update_channel(); }

	void enable_read()  { events_ |= (EVENT_READ | EVENT_CLOSE); update_channel(); }
	void enable_write() { events_ |= EVENT_WRITE; update_channel(); }

	/// requested events
	short events() const { return events_; }

	/// set returned events
	void set_revents(short events) { revents_ = events; }

	void set_read_callback(const EventCallback &cb) { read_cb_= cb; }
	void set_write_callback(const EventCallback &cb) { write_cb_ = cb; }
	void set_close_callback(const EventCallback &cb) { close_cb_ = cb; }
	void set_error_callback(const EventCallback &cb) { error_cb_ = cb; }

	int index() const { return index_; }
	void set_index(int index) { index_ = index; }
private:
	void update_channel();

	EventLoop *loop_;

	int fd_;
	short events_;  // requested events
	short revents_; // returned events	
	int index_; 

	EventCallback read_cb_;
	EventCallback write_cb_;
	EventCallback close_cb_;
	EventCallback error_cb_;
};

} // namespace net
} // namespace sduzh

#endif
