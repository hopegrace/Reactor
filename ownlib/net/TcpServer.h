#ifndef SDUZH_OWNLIB_NET_TCP_SERVER_H
#define SDUZH_OWNLIB_NET_TCP_SERVER_H

#include <functional>
#include <unordered_map>

#include <ownlib/net/Buffer.h>

namespace sduzh {
namespace net {

class Channel;
class EventLoop;
class InetAddress;
class TcpConnection;
class TcpSocket;

class TcpServer {
public:
	typedef std::function<void (TcpConnection*)> EventCallback;

	TcpServer(EventLoop *loop);
	~TcpServer();

	TcpServer(const TcpServer &) = delete;
	TcpServer &operator=(const TcpServer &) = delete;

	void start(const InetAddress &bind_addr);

	void set_connection_callback(const EventCallback &cb) { connection_cb_ = cb; }
	void set_message_callback(const EventCallback &cb) { message_cb_ = cb; }
	void set_write_complete_callback(const EventCallback &cb) { write_complete_cb_ = cb; }

private:
	void on_connection();

	/// called by TcpConnection
	void on_close(TcpConnection *conn);

	typedef std::unordered_map<int, TcpConnection*> ConnectionMap;

	EventLoop *loop_;
	TcpSocket *bind_socket_;
	// TODO replace with Acceptor
	Channel *bind_channel_;

	// user specifiy callbacks
	EventCallback connection_cb_;
	EventCallback write_complete_cb_; // TODO rename
	EventCallback message_cb_;

	ConnectionMap connections_;
};

} // namespace net
} // namespace sduzh

#endif
