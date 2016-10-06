#include <reactor/net/SocketOpt.h>
#include <reactor/base/SimpleLogger.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <reactor/net/InetAddress.h>

namespace sduzh {
namespace net {

int sockets::create_socket() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		LOG(Fatal) << "create_socket() " << ::strerror(errno);
	}
	return fd;
}

int sockets::create_nonblock_socket() {
	int fd = create_socket();
	set_blocking(fd, false);
	return fd;
}

int sockets::get_socket_error(int fd) {
	int err = 0;
	socklen_t optlen = sizeof(err);
	int ret = ::getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &optlen);
	return (ret < 0) ? ret : err;
}

int sockets::set_blocking(int fd, bool on) {
	int flag = ::fcntl(fd, F_GETFD, 0);
	if (flag < 0) {
		return flag;
	}

	if (!on) {
		flag |= O_NONBLOCK;
	} else {
		flag &= ~O_NONBLOCK;
	}

	return fcntl(fd, F_SETFD, flag);
}

int sockets::set_tcp_nodelay(int fd, bool on) {
	int optval = on ? 1 : 0;
	return ::setsockopt(fd, SOL_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

int sockets::set_reuse_addr(int fd, bool on) {
  int optval = on ? 1 : 0;
  return ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
}

int sockets::set_reuse_port(int fd, bool on) {
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  return ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
#endif
}

int sockets::connect(int fd, const InetAddress &addr) {
	struct sockaddr_in sockaddr = addr.sockaddr();
	return ::connect(fd, reinterpret_cast<struct sockaddr*>(&sockaddr), sizeof sockaddr);
}

} // namespace net
} // namespace sduzh
