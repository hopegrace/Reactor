#ifndef SDUZH_REACTOR_NET_CONNECTOR_H
#define SDUZH_REACTOR_NET_CONNECTOR_H

#include <memory>
#include <reactor/net/Channel.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpSocket.h>

namespace sduzh {
namespace net {

class Connector {
public:
	Connector(EventLoop *loop, const InetAddress &peer);
	~Connector();

	void connect();

private:
	typedef std::unique_ptr<TcpSocket> SocketPtr;
	typedef std::unique_ptr<Channel> ChannelPtr;

	enum StateE {
		Disconnected, 
		Connecting,
		Connected,
	};

	EventLoop *loop_;
	SocketPtr socket_;
	ChannelPtr channel_;

	StateE state_;
};

} // namespace net
} // namespace sduzh

#endif
