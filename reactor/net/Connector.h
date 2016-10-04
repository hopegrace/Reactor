#ifndef SDUZH_REACTOR_NET_CONNECTOR_H
#define SDUZH_REACTOR_NET_CONNECTOR_H

#include <functional>
#include <memory>
#include <reactor/net/Channel.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpSocket.h>

namespace sduzh {
namespace net {

class Connector {
public:
	Connector(EventLoop *loop, const InetAddress &servaddr);
	~Connector();

	void connect();

private:
	typedef std::unique_ptr<TcpSocket> SocketPtr;
	typedef std::unique_ptr<Channel> ChannelPtr;

	enum class State {
		Disconnected, 
		Connecting,
		Connected,
	};

	void do_connect();
	void on_write();

	EventLoop *loop_;
	InetAddress servaddr_;
	SocketPtr socket_;
	ChannelPtr channel_;

	State state_;
};

} // namespace net
} // namespace sduzh

#endif
