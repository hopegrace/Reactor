#ifndef SDUZH_OWNLIB_NET_TCP_CONNECTION_H
#define SDUZH_OWNLIB_NET_TCP_CONNECTION_H

#include <ownlib/net/Buffer.h>
#include <ownlib/net/Channel.h>
#include <ownlib/net/TcpSocket.h>

namespace sduzh {
namespace net {

class EventLoop;

class TcpConnection {
public:
	typedef std::function<void (TcpConnection *, Buffer *)> MessageCallback;
	typedef std::function<void (TcpConnection *)> WriteCompleteCallback;

	TcpConnection(EventLoop *loop, int fd);
	~TcpConnection();

	TcpConnection(const TcpConnection &) = delete;
	TcpConnection &operator=(const TcpConnection &) = delete;

	bool connected() const;

	void close();
	void shutdown_read();
	void shutdown_write();

	void write(const char *buffer, size_t len);

	void set_message_callback(const MessageCallback &cb) { msg_cb_ = cb; }
	void set_write_complete_callback(const WriteCompleteCallback &cb) { write_complete_cb_ = cb; }

private:
	void on_readable(int fd);
	void on_writable(int fd);

	EventLoop *loop_;
	TcpSocket socket_;
	Channel channel_;
	MessageCallback msg_cb_;
	WriteCompleteCallback write_complete_cb_;
	Buffer read_buffer_;
	Buffer write_buffer_;
}; 
} // namespace net
} // nemespace sduzh

#endif
