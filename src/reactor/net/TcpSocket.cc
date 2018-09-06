#include "reactor/net/TcpSocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <reactor/base/SimpleLogger.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/SocketOpt.h>

using namespace reactor::base;

namespace reactor {
namespace net {

TcpSocket::TcpSocket() {
	sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_ < 0) {
		perror("socket");
	}
}

TcpSocket::TcpSocket(int fd) : sockfd_(fd) {
}

TcpSocket::~TcpSocket() {
	::close(sockfd_);
}

int TcpSocket::accept(InetAddress *peer) {
	struct sockaddr_in peeraddr;
	socklen_t socklen = sizeof(peeraddr);
	int fd = ::accept(sockfd_, reinterpret_cast<struct sockaddr*>(&peeraddr), &socklen);
	if (fd >= 0 && peer) {
		peer->set_address(peeraddr);
	}
	return fd;
}

void TcpSocket::bind(const InetAddress &address) {
	struct sockaddr_in sockaddr = address.sockaddr();
	int ret = ::bind(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
	if (ret < 0) {
		perror("bind");
		abort();
	}
}

void TcpSocket::close() {
	::close(sockfd_);
}

int TcpSocket::connect(const InetAddress &address) {
	struct sockaddr_in sockaddr = address.sockaddr();
	return ::connect(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof(sockaddr));
}

InetAddress TcpSocket::getpeername() const {
	struct sockaddr_in sockaddr;
	socklen_t socklen = sizeof sockaddr;
	::getpeername(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	return InetAddress(sockaddr);
}

InetAddress TcpSocket::getsockname() const {
	struct sockaddr_in sockaddr;
	socklen_t socklen = sizeof sockaddr;
	::getsockname(sockfd_, reinterpret_cast<struct sockaddr*>(&sockaddr), &socklen);
	return InetAddress(sockaddr);
}

void TcpSocket::listen(int backlog) {
	int ret = ::listen(sockfd_, backlog);
	if (ret < 0) {
		perror("listen");
		abort();
	}
}

ssize_t TcpSocket::recv(char *buff, size_t max_len) {
	return ::recv(sockfd_, buff, max_len, 0);
}

ssize_t TcpSocket::send(const char *data) {
	return this->send(data, strlen(data));
}

ssize_t TcpSocket::send(const char *data, size_t len) {
	return ::send(sockfd_, data, len, 0);
}

ssize_t TcpSocket::send(const string_t &data) {
	return this->send(data.c_str(), data.size());
}

void TcpSocket::sendall(const char *data) {
	sendall(data, strlen(data));
}

void TcpSocket::sendall(const string_t &data) {
	sendall(data.c_str(), data.size());
}

void TcpSocket::sendall(const char *data, size_t len) {
	size_t nwrite = 0;
	while (nwrite < len) {
		ssize_t nw = ::send(sockfd_, data+nwrite, (len-nwrite), 0);
		if (nw < 0)
			return;
		nwrite += nw;
	}
}

void TcpSocket::shutdownread() {
	::shutdown(sockfd_, SHUT_RD);
}

void TcpSocket::shutdownwrite() {
	::shutdown(sockfd_, SHUT_WR);
}

void TcpSocket::set_blocking(bool block) {
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

void TcpSocket::set_reuse_addr(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void TcpSocket::set_reuse_port(bool on) {
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {
	LOG(Error) << "SO_REUSEPORT failed";
  }
#endif
}

void TcpSocket::set_keep_alive(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

int TcpSocket::get_error() const {
	int err = 0;
	socklen_t optlen = sizeof(err);
	int ret = ::getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &err, &optlen);
	if (ret) {
		LOG(Error) << "in TcpSocket::get_error " << ret;
	}
	return err;
}

bool TcpSocket::get_tcp_info(struct tcp_info* tcpi) const
{
	socklen_t len = sizeof(*tcpi);
	bzero(tcpi, len);
	return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool TcpSocket::get_tcp_info_string(char* buf, int len) const
{
	struct tcp_info tcpi;
	bool ok = get_tcp_info(&tcpi);
	if (ok)
	{
	  snprintf(buf, len, "unrecovered=%u "
	           "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
	           "lost=%u retrans=%u rtt=%u rttvar=%u "
	           "sshthresh=%u cwnd=%u total_retrans=%u",
	           tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
	           tcpi.tcpi_rto,          // Retransmit timeout in usec
	           tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
	           tcpi.tcpi_snd_mss,
	           tcpi.tcpi_rcv_mss,
	           tcpi.tcpi_lost,         // Lost packets
	           tcpi.tcpi_retrans,      // Retransmitted packets out
	           tcpi.tcpi_rtt,          // Smoothed round trip time in usec
	           tcpi.tcpi_rttvar,       // Medium deviation
	           tcpi.tcpi_snd_ssthresh,
	           tcpi.tcpi_snd_cwnd,
	           tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
	}
	return ok;
}

} // namespace net
} // namespace reactor
