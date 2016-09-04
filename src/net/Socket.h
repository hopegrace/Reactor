#ifndef SDUZH_TOOLS_SOCKET_H
#define SDUZH_TOOLS_SOCKET_H

#include <string>
#include <sys/types.h>

namespace sduzh {
namespace net {

class InetAddress;

class Socket {
public:
	typedef std::string string_t;

	/// will create a socket 
	Socket();
	Socket(int fd);
	~Socket();

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
	ssize_t send(const char *data, size_t len);
	void sendall(const char *data, size_t len);	
	void setblocking(bool block);
	void shutdownread();
	void shutdownwrite();

private:
	int sockfd_;
};

} // namespace net
} // namespace sduzh

#endif
