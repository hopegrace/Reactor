#ifndef SDUZH_OWNLIB_NET_TCP_CONNECTION_H
#define SDUZH_OWNLIB_NET_TCP_CONNECTION_H

#include <memory>
#include <reactor/net/Buffer.h>
#include <reactor/net/Callbacks.h>
#include <reactor/net/Channel.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpSocket.h>

namespace sduzh {
namespace net {

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	TcpConnection(EventLoop *loop, int fd);
	~TcpConnection();

	/// disable copy 
	TcpConnection(const TcpConnection &) = delete;
	TcpConnection &operator=(const TcpConnection &) = delete;

	int fd() const { return socket_.fd(); }
	bool connected() const { return state_ == kConnected; }
	bool disconnected() const { return state_ == kDisconnected; }
	bool get_tcp_info(struct tcp_info *) const;
	std::string get_tcp_info_string() const;

	void set_tcp_nodelay(bool on);

	const InetAddress &peer_address() const { return peer_addr_; }
	const InetAddress &local_address() const { return local_addr_; }

	/// force close, not wait for write complete
	void abort();
	/// will wait for write complete
	void close();

	Buffer *message() { return &input_buffer_; }
	void write(const char *buffer, size_t len);

	void set_connection_callback(const ConnectionCallback &cb) { connection_cb_ = cb; } 
	void set_write_complete_callback(const WriteCompleteCallback &cb) { write_complete_cb_ = cb; }
	void set_message_callback(const MessageCallback &cb) { message_cb_ = cb; }
	void set_close_callback(const CloseCallback &cb) { close_cb_ = cb; }

	void connection_established();

private:
	void on_read();
	void on_write();
	void on_close();
	void on_error();

	enum StateE { kConnecting, kConnected, kDisconnected, kDisconnecting };

	void set_state(StateE s) { state_ = s; }
	std::string str_state(StateE s);

	EventLoop *loop_;
	TcpSocket socket_;
	Channel channel_;
	StateE state_;

	ConnectionCallback    connection_cb_;
	MessageCallback       message_cb_;
	WriteCompleteCallback write_complete_cb_;
	CloseCallback         close_cb_;

	Buffer input_buffer_;
	Buffer output_buffer_;

	InetAddress local_addr_;
	InetAddress peer_addr_;
}; 

} // namespace net
} // nemespace sduzh

#endif
