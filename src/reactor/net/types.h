#ifndef SDUZH_NET_TYPES_H
#define SDUZH_NET_TYPES_H

#include <memory>

namespace reactor {
namespace net {

#define EVENT_NONE    0
#define EVENT_READ    1
#define EVENT_WRITE   2
#define EVENT_ERROR	  4
#define EVENT_CLOSE   8 


class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

} // namespace net
} // namespace reactor

#endif 
