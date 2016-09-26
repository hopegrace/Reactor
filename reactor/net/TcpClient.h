#ifndef SDUZH_REACTOR_NET_TCP_CLIENT_H
#define SDUZH_REACTOR_NET_TCP_CLIENT_H

#include <functional>

#include <reactor/net/Channel.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TcpConnection.h>
#include <reactor/net/TcpSocket.h>

namespace sduzh {
namespace net {

class TcpClient {
public:
	typedef std::function<void (const TcpConnectionPtr &)> EventCallback;

	TcpClient(EventLoop *loop);

	/// TODO non-block
	void connect(const InetAddress &);

	void send(const char *data, size_t len);

	void set_connection_callback(const EventCallback &cb) { 
		conn_->set_connection_callback(cb);
	}

	void set_message_callback(const EventCallback &cb) {  
		conn_->set_message_callback(cb);
	}

	void set_write_complete_callback(const EventCallback &cb) { 
		conn_->set_write_complete_callback(cb);
	}

	// error callback?

private:
	void on_close(const TcpConnectionPtr &);

	EventLoop *loop_;
	TcpSocket socket_;
	TcpConnectionPtr conn_;
};

} // namespace net
} // namespace sduzh

#endif
