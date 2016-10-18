#ifndef SDUZH_REACTOR_NET_CONNECTOR_H
#define SDUZH_REACTOR_NET_CONNECTOR_H

#include <functional>
#include <memory>
#include <reactor/net/Channel.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpSocket.h>

namespace reactor {
namespace net {

class Connector {
public:
	typedef std::function<void (int)> ConnectionCallback;

	Connector(EventLoop *loop, const InetAddress &servaddr);
	~Connector();

	void set_connection_callback(const ConnectionCallback &cb) {
		connection_cb_ = cb;
	}

	void restart();
	void start();
	stop();

private:
	enum class State {
		Disconnected, 
		Connecting,
		Connected,
	};

	const char *str_state();

	void on_error();
	void on_write();

	void close_socket();
	void delete_channel();
	void connect();
	void connecting();
	void retry();
	
	void _delete_channel();

	EventLoop *loop_;
	InetAddress servaddr_;
	int socket_;
	Channel *channel_;

	State state_;
	
	int delay_ms_;

	ConnectionCallback connection_cb_;
};

} // namespace net
} // namespace reactor

#endif
