#include <reactor/net/SocketOpt.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace sduzh {
namespace net {

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

} // namespace net
} // namespace sduzh
