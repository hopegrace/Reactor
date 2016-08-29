#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace socket
{

Socket::Socket() {
	sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
	::close(sockfd_);
}

Socket Socket::accept() {
	Socket client;
	struct sockaddr peeraddr;
	client.sockfd_ = ::accept(sockfd_, &peeraddr, sizeof(peeraddr));
	return client;
}

int Socket::bind(const SockAddress &address) {
	struct sockaddr_in sockaddr = address.sockaddr();
	return ::bind(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
}

void Socket::close() {
	::close(sockfd_);
}

int Socket::connect(const SockAddress &address) {
	struct sockaddr_in sockaddr = address.sockaddr();
	return ::connect(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
}

Socket Socket::dup() {
	Socket ret;
	ret.sockfd_ = sockfd_;
	ret.timeout_ = timeout_;
	return ret;
}


SockAddress Socket::getpeername() const {
	struct sockaddr_in sockaddr;
	::getpeername(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
	return SockAddress(sockaddr);
}

SockAddress Socket::getsockname() const {
	struct sockaddr_in sockaddr;
	::getsockname(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
	return SockAddress(sockaddr);
}

int Socket::listen(int backlog) {
	return ::listen(sockfd_, backlog);
}

size_t Socket::recv(char *buff, size_t max_len) {
	return ::recv(sockfd_, buff, max_len, 0);
}

ssize_t Socket::send(const char *data, size_t len) {
	return ::send(sockfd_, data, len, 0);
}

void Socket::sendall(const char *data, size_t len) {
	size_t nwrite = 0;
	while (nwrite < len) {
		ssize_t nw = ::send(sockfd_, data+nwrite, (len-nwrite), 0);
		if (nw < 0)
			return;
		nwrite += nw;
	}
}

void Socket::setblocking(bool flag) {
}

void Socket::settimeout(int timeout) {
	timeout_ = timeout;
}

void Socket::shutdownread() {
	::shutdown(sockfd_, SHUT_RD);
}

void Socket::shutdownwrite() {
	::shutdown(sockfd_, SHUT_WR);
}

} // namespace socket
