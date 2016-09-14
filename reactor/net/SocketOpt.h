#ifndef SDUZH_OWNLIB_NET_SOCKET_OPT_H
#define SDUZH_OWNLIB_NET_SOCKET_OPT_H

namespace sduzh {
namespace net {

namespace SocketOpt {

void set_blocking(int fd, bool on);
void reuse_addr(int fd, bool on);
void reuse_port(int fd, bool on);

} // namespace SocketOpt

} // namespace net
} // namespace sduzh

#endif
