#ifndef SDUZH_OWNLIB_NET_TCP_CONNECTION_H
#define SDUZH_OWNLIB_NET_TCP_CONNECTION_H

#include <ownlib/net/Buffer.h>
#include <ownlib/net/Channel.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/TcpSocket.h>

namespace sduzh {
namespace net {

class EventLoop;

class TcpConnection {
public:
	typedef std::function<void (TcpConnection*)> EventCallback;

	TcpConnection(EventLoop *loop, int fd);
	~TcpConnection();

	/// disable copy 
	TcpConnection(const TcpConnection &) = delete;
	TcpConnection &operator=(const TcpConnection &) = delete;

	int fd() const { return socket_.fd(); }
	bool connected() const { return state_ == kConnected; }

	Buffer *message() { return &read_buffer_; }

	const InetAddress &peer_address() const { return peer_addr_; }
	const InetAddress &local_address() const { return local_addr_; }

	/// force close, not wait for write complete
	void abort();
	/// will wait for write complete
	void close();

	void write(const char *buffer, size_t len);

	void set_connection_callback(const EventCallback &cb) { connection_cb_ = cb; } 
	void set_write_complete_callback(const EventCallback &cb) { write_complete_cb_ = cb; }
	void set_message_callback(const EventCallback &cb) { message_cb_ = cb; }
	void set_close_callback(const EventCallback &cb) { close_cb_ = cb; }

	void connection_established();

private:
	void on_read();
	void on_write();
	void on_close();
	void on_error();

	enum StateE { kConnecting, kConnected, kDisconnected, kDisconnecting };

	void set_state(StateE s) { state_ = s; }

	EventLoop *loop_;
	TcpSocket socket_;
	Channel channel_;
	StateE state_;

	EventCallback connection_cb_;
	EventCallback message_cb_;
	EventCallback write_complete_cb_;
	EventCallback close_cb_;

	Buffer read_buffer_;
	Buffer write_buffer_;

	InetAddress local_addr_;
	InetAddress peer_addr_;
}; 
} // namespace net
} // nemespace sduzh

#endif
