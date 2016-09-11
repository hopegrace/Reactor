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
	typedef std::function<void (TcpConnection *)> DisconnectedCallback;
	typedef std::function<void (TcpConnection *)> WriteCompleteCallback;
	typedef std::function<void (TcpConnection *, Buffer *)> MessageCallback;

	TcpConnection(EventLoop *loop, int fd);
	~TcpConnection();

	/// disable copy 
	TcpConnection(const TcpConnection &) = delete;
	TcpConnection &operator=(const TcpConnection &) = delete;

	int fd() const { return socket_.fd(); }
	bool connected() const { return connected_; }

	InetAddress peer_address() const { return socket_.getpeername(); }
	InetAddress local_address() const { return socket_.getsockname(); }

	void close();
	void shutdown_read();
	void shutdown_write();

	void write(const char *buffer, size_t len);

	void set_disconnected_callback(const DisconnectedCallback &cb) { disconnected_cb_ = cb; }
	void set_write_complete_callback(const WriteCompleteCallback &cb) { write_complete_cb_ = cb; }
	void set_message_callback(const MessageCallback &cb) { message_cb_ = cb; }

private:
	void on_readable(int fd);
	void on_writable(int fd);

	void handle_closed();

	EventLoop *loop_;
	TcpSocket socket_;
	Channel channel_;

	bool connected_;

	DisconnectedCallback  disconnected_cb_;
	WriteCompleteCallback write_complete_cb_;
	MessageCallback       message_cb_;

	Buffer read_buffer_;
	Buffer write_buffer_;
}; 
} // namespace net
} // nemespace sduzh

#endif
