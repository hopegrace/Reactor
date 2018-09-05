#ifndef SDUZH_OWNLIB_NET_SOCKET_OPT_H
#define SDUZH_OWNLIB_NET_SOCKET_OPT_H

namespace reactor {
namespace net {

class InetAddress;

namespace sockets {

int create_socket();
int create_nonblock_socket();

int get_socket_error(int fd);

int set_blocking(int fd, bool on);
int set_tcp_nodelay(int fd, bool on);
int set_reuse_addr(int fd, bool on);
int set_reuse_port(int fd, bool on);

void set_nonblock_or_die(int fd);

int connect(int fd, const InetAddress &addr);

} // namespace sockets
} // namespace net
} // namespace reactor

#endif
