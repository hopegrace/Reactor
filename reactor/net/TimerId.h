#ifndef SDUZH_REACTOR_NET_TIMER_ID_H
#define SDUZH_REACTOR_NET_TIMER_ID_H

#include <memory>

namespace sduzh {
namespace net {

class Timer;

typedef std::weak_ptr<Timer> TimerId;

} // namespace net
} // namespace sduzh

#endif
