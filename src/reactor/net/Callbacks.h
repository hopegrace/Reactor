#ifndef SDUZH_OWNLIB_NET_CALLBACKS_H
#define SDUZH_OWNLIB_NET_CALLBACKS_H

#include <functional>
#include <memory>

#include "types.h"

namespace reactor {
namespace net {

typedef std::function<void (const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr &)> MessageCallback;
typedef std::function<void (const TcpConnectionPtr &)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr &)> CloseCallback;

typedef std::function<void ()> TimerCallback;

} // namespace net
} // namespace reactor

#endif
