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
	typedef std::function<void (TcpConnection*)> 			ConnectionCallback;
	typedef std::function<void (TcpConnection*)>            WriteCompleteCallback;
	typedef std::function<void (TcpConnection*, Buffer *)>  MessageCallback;

	TcpServer(EventLoop *loop);
	~TcpServer();

	TcpServer(const TcpServer &) = delete;
	TcpServer &operator=(const TcpServer &) = delete;

	void start(const InetAddress &bind_addr);

	void set_connection_callback(const ConnectionCallback &cb) { conn_cb_ = cb; }
	void set_write_complete_callback(const WriteCompleteCallback &cb) { write_complete_cb_ = cb; }
	void set_message_callback(const MessageCallback &cb) { msg_cb_ = cb; }

private:
	void on_new_connection(int fd);
	void on_disconnected(TcpConnection *conn);

	typedef std::unordered_map<int, TcpConnection*> ConnectionMap;
	EventLoop *loop_;
	TcpSocket *bind_socket_;
	Channel *bind_channel_;

	ConnectionCallback conn_cb_;
	WriteCompleteCallback write_complete_cb_; // TODO rename
	MessageCallback msg_cb_;

	ConnectionMap connections_;
};

} // namespace net
} // namespace sduzh

#endif
