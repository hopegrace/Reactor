#ifndef SDUZH_TOOLS_SOCKET_H
#define SDUZH_TOOLS_SOCKET_H

#include <string>

#include <sys/types.h>

namespace socket
{

typedef string_t std::string;

class SockAddress {
public:
	SockAddress();
	SockAddress(const string_t &host, uint16_t port);
	SockAddress(const struct sockaddr_in &sockaddr);

	SockAddress(const SockAddress &) = default;
	SockAddress & operator=(const SockAddress &) = default;
	
	string_t host() const { return host_; }
	uint16_t port() const { return port_; }

	struct sockaddr_in sockaddr() const;

private:
	string_t host_;
	uint16_t port_;
	struct sockaddr_in sockaddr_;
};

class Socket {
public:
	Socket();
	~Socket();

	Socket(const Socket &) = delete;
	Socket & operator=(const Socket &) = delete;

	Socket accept();
	int bind(const SockAddress &addr);
	void close();
	int connect(const SockAddress &addr);
	Socket dup() const;
	int fileno() const { 
		return sockfd_; 
	}
	SockAddress getpeername() const;
	SockAddress getsockname() const;
	int gettimeout() const {
		return timeout_; 
	}
	int listen(int backlog=5);
	size_t recv(char *buff, size_t max_len);
	ssize_t send(const char *data, size_t len);
	void sendall(const char *data, size_t len);	
	void setblocking(bool flag);
	void settimeout(int timeout);
	void shutdownread();
	void shutdownwrite();

private:
	void setsockfd(int fd);

	int sockfd_;
	int timeout_;
};

} // namespace socket

#endif
