#ifndef SDUZH_OWNLIB_NET_TCP_SERVER_H
#define SDUZH_OWNLIB_NET_TCP_SERVER_H

#include <functional>
#include <unordered_map>

#include <reactor/net/Buffer.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/Callbacks.h>

namespace sduzh {
namespace net {

class Channel;
class EventLoop;
class InetAddress;
class TcpConnection;
class TcpSocket;

class TcpServer {
public:
	TcpServer(EventLoop *loop, const InetAddress &addr);
	~TcpServer();

	TcpServer(const TcpServer &) = delete;
	TcpServer &operator=(const TcpServer &) = delete;

	/// default true
	void set_reuse_addr(bool on);
	
	void start();

	void set_connection_callback(const ConnectionCallback &cb) { connection_cb_ = cb; }
	void set_message_callback(const MessageCallback &cb) { message_cb_ = cb; }
	void set_write_complete_callback(const WriteCompleteCallback &cb) { write_complete_cb_ = cb; }

private:
	void on_connection();

	/// called by TcpConnection
	void on_close(const TcpConnectionPtr &conn);

	typedef std::unordered_map<int, TcpConnectionPtr> ConnectionMap;

	EventLoop *loop_;
	InetAddress bind_addr_;
	TcpSocket *bind_socket_;
	// TODO replace with Acceptor
	Channel *bind_channel_;

	// user specifiy callbacks
	ConnectionCallback    connection_cb_;
	MessageCallback 	  message_cb_;
	WriteCompleteCallback write_complete_cb_; 

	ConnectionMap connections_;
};

} // namespace net
} // namespace sduzh

#endif
