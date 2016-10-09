#ifndef SDUZH_REACTOR_NET_TCP_CLIENT_H
#define SDUZH_REACTOR_NET_TCP_CLIENT_H

#include <reactor/net/Connector.h>
#include <reactor/net/TcpConnection.h>

namespace reactor {
namespace net {

class EventLoop;
class InetAddress;

class TcpClient {
public:
	TcpClient(EventLoop *loop, const InetAddress &servaddr);

	void connect();
	void disconnect();
	void stop();

	TcpConnectionPtr connection() const { return connection_; }

	EventLoop *loop() const { return loop_; }

	void set_connection_callback(const ConnectionCallback &cb) { 
		connection_cb_ = cb;
	}

	void set_message_callback(const MessageCallback &cb) {  
		message_cb_ = cb;
	}

	void set_write_complete_callback(const WriteCompleteCallback &cb) { 
		write_complete_cb_ = cb;
	}

private:
	typedef std::unique_ptr<Connector> ConnectorPtr;
	
	void on_connected(int fd);
	void on_close(const TcpConnectionPtr &);

	EventLoop *loop_;
	ConnectorPtr connector_;
	TcpConnectionPtr connection_;

	ConnectionCallback connection_cb_;
	WriteCompleteCallback write_complete_cb_;
	MessageCallback message_cb_;
};

} // namespace net
} // namespace reactor

#endif
