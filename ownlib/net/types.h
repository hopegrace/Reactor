#ifndef SDUZH_NET_TYPES_H
#define SDUZH_NET_TYPES_H

namespace sduzh {
namespace net {

#define OVERRIDE override

#define EVENT_NONE       0
#define EVENT_READABLE   1
#define EVENT_WRITABLE   2
#define EVENT_ERROR      4

struct PollEvent {
	int fd; 
	short events;

	PollEvent(): fd(-1), events(EVENT_NONE) {
	}
}; 

} // namespace net
} // namespace sduzh

#endif 
