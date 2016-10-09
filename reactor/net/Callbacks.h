#ifndef SDUZH_OWNLIB_NET_CALLBACKS_H
#define SDUZH_OWNLIB_NET_CALLBACKS_H

#include <functional>
#include <memory>

namespace reactor {
namespace net {

class TcpConnection;

typedef std::shared_ptr<TcpConnection>  TcpConnectionPtr;

typedef std::function<void (const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr &)> MessageCallback;
typedef std::function<void (const TcpConnectionPtr &)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr &)> CloseCallback;

typedef std::function<void ()> TimerCallback;

} // namespace net
} // namespace reactor

#endif
