#ifndef SDUZH_OWNLIB_NET_CHANNEL_H
#define SDUZH_WONLIB_NET_CHANNEL_H

#include <functional>

#include <ownlib/net/types.h>

namespace sduzh {
namespace net {

class EventLoop;

class Channel {
public:
	typedef std::function<void (int)> ReadCallback;
	typedef std::function<void (int)> WriteCallback;
	typedef std::function<void (int)> ErrorCallback;

	Channel(EventLoop *loop, int fd);
	~Channel();
	
	Channel(const Channel &) = delete;
	Channel &operator=(const Channel &) = delete;

	int fd() const { return fd_; }

	void process_events();

	void disable_read()  { events_ &= ~EVENT_READABLE; update_channel(); }
	void disable_write() { events_ &= ~EVENT_WRITABLE; update_channel(); }
	void disable_error() { events_ &= ~EVENT_ERROR; update_channel(); }

	void enable_read()  { events_ |= EVENT_READABLE; update_channel(); }
	void enable_write() { events_ |= EVENT_WRITABLE; update_channel(); }
	void enable_error() { events_ |= EVENT_ERROR; update_channel(); }

	/// requested events
	short events() const { return events_; }

	/// set returned events
	void set_revents(short events) { revents_ = events; }

	void set_read_callback(const ReadCallback &cb) { read_callback_ = cb; }
	void set_write_callback(const WriteCallback &cb) { write_callback_ = cb; }
	void set_error_callback(const ErrorCallback &cb) { error_callback_ = cb; }

	int index() const { return index_; }
	void set_index(int index) { index_ = index; }
private:
	void update_channel();

	EventLoop *loop_;

	int fd_;
	short events_;  // requested events
	short revents_; // returned events	
	int index_; 

	ReadCallback  read_callback_;
	WriteCallback write_callback_;
	ErrorCallback error_callback_;
};

} // namespace net
} // namespace sduzh

#endif
