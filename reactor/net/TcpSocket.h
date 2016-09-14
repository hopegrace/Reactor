#ifndef SDUZH_OWNLIB_NET_TCP_SOCKET_H
#define SDUZH_OWNLIB_NET_TCP_SOCKET_H

#include <string>
#include <sys/types.h>

#include <reactor/net/InetAddress.h>


namespace sduzh {
namespace net {

class TcpSocket {
public:
	typedef std::string string_t;

	/// will create a socket 
	TcpSocket();
	TcpSocket(int fd);
	~TcpSocket();

	TcpSocket(const TcpSocket &) = delete;
	TcpSocket & operator = (const TcpSocket &) = delete;

	int accept(InetAddress *peer);
	/// abort if error
	void bind(const InetAddress &addr);
	void close();
	int connect(const InetAddress &addr);
	int fd() const { 
		return sockfd_; 
	}
	InetAddress getpeername() const;
	InetAddress getsockname() const;
	/// abort if error
	void listen(int backlog=5);
	ssize_t recv(char *buff, size_t max_len);
	/// send string 
	ssize_t send(const char *data);
	ssize_t send(const char *data, size_t len);
	ssize_t send(const string_t &data);
	void sendall(const char *data);
	void sendall(const char *data, size_t len);	
	void sendall(const string_t &data);
	void shutdownread();
	void shutdownwrite();

	/// abort if failed
	void set_blocking(bool block);
	void set_reuse_addr(bool on);
	void set_reuse_port(bool on);
	void set_keep_alive(bool on);

private:
	int sockfd_;
};

} // namespace net
} // namespace sduzh

#endif
