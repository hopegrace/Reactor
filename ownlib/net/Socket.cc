#include "Socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "InetAddress.h"

namespace sduzh {
namespace net {

Socket::Socket() {
	sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_ < 0) {
		perror("socket");
	}
}

Socket::Socket(int fd) : sockfd_(fd) {
}

Socket::~Socket() {
	::close(sockfd_);
}

int Socket::accept(InetAddress *peer) {
	struct sockaddr_in peeraddr;
	socklen_t socklen = sizeof(peeraddr);
	int fd = ::accept(sockfd_, reinterpret_cast<struct sockaddr*>(&peeraddr), &socklen);
	if (fd >= 0 && peer) {
		peer->set_address(peeraddr);
	}
	return fd;
}

void Socket::bind(const InetAddress &address) {
	struct sockaddr_in sockaddr = address.sockaddr();
	int ret = ::bind(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
	if (ret < 0) {
		perror("bind");
		abort();
	}
}

void Socket::close() {
	::close(sockfd_);
}

int Socket::connect(const InetAddress &address) {
	struct sockaddr_in sockaddr = address.sockaddr();
	return ::connect(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
}

InetAddress Socket::getpeername() const {
	struct sockaddr_in sockaddr;
	socklen_t socklen = sizeof sockaddr;
	::getpeername(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	return InetAddress(sockaddr);
}

InetAddress Socket::getsockname() const {
	struct sockaddr_in sockaddr;
	socklen_t socklen = sizeof sockaddr;
	::getsockname(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	return InetAddress(sockaddr);
}

void Socket::listen(int backlog) {
	int ret = ::listen(sockfd_, backlog);
	if (ret < 0) {
		perror("listen");
		abort();
	}
}

ssize_t Socket::recv(char *buff, size_t max_len) {
	return ::recv(sockfd_, buff, max_len, 0);
}

ssize_t Socket::send(const char *data) {
	return this->send(data, strlen(data));
}

ssize_t Socket::send(const char *data, size_t len) {
	return ::send(sockfd_, data, len, 0);
}

ssize_t Socket::send(const string_t &data) {
	return this->send(data.c_str(), data.size());
}

void Socket::sendall(const char *data) {
	sendall(data, strlen(data));
}

void Socket::sendall(const string_t &data) {
	sendall(data.c_str(), data.size());
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

void Socket::shutdownread() {
	::shutdown(sockfd_, SHUT_RD);
}

void Socket::shutdownwrite() {
	::shutdown(sockfd_, SHUT_WR);
}

void Socket::set_blocking(bool block) {
	int flag = fcntl(sockfd_, F_GETFD, 0);
	if (flag < 0) {
		perror("fcntl");
		abort();
	}

	if (!block) {
		flag |= O_NONBLOCK;
	} else {
		flag &= ~O_NONBLOCK;
	}

	if (fcntl(sockfd_, F_SETFD, flag) < 0) {
		perror("fcntl");
		abort();
	}
}

void Socket::set_reuse_addr(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::set_reuse_port(bool on) {
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {
    fprintf(stderr, "SO_REUSEPORT failed\n");
  }
#endif
}

void Socket::set_keep_alive(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

} // namespace net
} // namespace sduzh
