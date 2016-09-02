#ifndef SDUZH_TOOLS_SOCKET_H
#define SDUZH_TOOLS_SOCKET_H

#include <string>
#include <sys/types.h>

class SockAddress;

class Socket {
public:
	typedef std::string string_t;

	Socket();
	Socket(int fd);
	~Socket();

	Socket(const Socket &) = default;
	Socket & operator=(const Socket &) = default;

	Socket accept();
	bool bind(const SockAddress &addr);
	void close();
	int connect(const SockAddress &addr);
	int fileno() const { 
		return sockfd_; 
	}
	SockAddress getpeername() const;
	SockAddress getsockname() const;
	int gettimeout() const {
		return timeout_; 
	}
	bool listen(int backlog=5);
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

#endif
