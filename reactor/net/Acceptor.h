#ifndef SDUZH_REACTOR_NET_ACCEPTOR_H
#define SDUZH_REACTOR_NET_ACCEPTOR_H

#include <functional>

#include <reactor/base/Timestamp.h>
#include <reactor/net/Channel.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpSocket.h>

namespace sduzh {
namespace net {

class DateTime;
class EventLoop;

class Acceptor {
public:
	typedef std::function<void (int fd, const InetAddress &addr, Timestamp time)> EventCallback;

	Acceptor(EventLoop *loop, const InetAddress &bind_addr);
	~Acceptor() = default;

	Acceptor(const Acceptor &) = delete;
	Acceptor &operator=(const Acceptor &) = delete;

	void listen();

	void set_new_connection_callback(const EventCallback &cb) {
		new_connection_cb_ = cb; 
	}

private:
	void on_read();

	EventLoop *loop_;
	TcpSocket socket_;
	Channel channel_;
	int idle_fd_;

	bool started_;

	EventCallback new_connection_cb_;
};

} // namespace net
} // namespace sduzh

#endif
