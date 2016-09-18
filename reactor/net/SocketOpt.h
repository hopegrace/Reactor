#ifndef SDUZH_OWNLIB_NET_SOCKET_OPT_H
#define SDUZH_OWNLIB_NET_SOCKET_OPT_H

namespace sduzh {
namespace net {
namespace sockets {

int set_blocking(int fd, bool on);
int set_tcp_nodelay(int fd, bool on);
int set_reuse_addr(int fd, bool on);
int set_reuse_port(int fd, bool on);

void set_nonblock_or_die(int fd);

} // namespace sockets
} // namespace net
} // namespace sduzh

#endif
