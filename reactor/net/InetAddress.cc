#include "InetAddress.h"
#include <string.h>
#include <strings.h>
#include <sstream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <reactor/base/SimpleLogger.h>

using namespace std;

namespace reactor {

namespace detail {

static bool resolve_addr(const std::string &host, const std::string &port, struct sockaddr_in *res) {
	struct addrinfo hints;
	struct addrinfo *result = NULL;
	::bzero(&hints, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	int ret = ::getaddrinfo(&*host.begin(), &*port.begin(), &hints, &result);
	if (ret == 0) {
		for (struct addrinfo *p = result; p; p = p->ai_next) {
			if (p->ai_family == AF_INET && p->ai_socktype == SOCK_STREAM 
					&& p->ai_addrlen == sizeof(struct sockaddr_in)) {
				*res = *reinterpret_cast<struct sockaddr_in*>(p->ai_addr);
			}
		}
	} else {
		LOG(Error) << ::strerror(errno);
	}

	::freeaddrinfo(result);

	return ret == 0;
}

} 

namespace net {

typedef InetAddress::string_t string_t;

InetAddress::InetAddress(): valid_(false), host_(), port_() {
	bzero(&sockaddr_, sizeof sockaddr_);
}

InetAddress::InetAddress(const string_t &host, uint16_t port) : 
		valid_(false), host_(host), port_(port) {
	//bzero(&sockaddr_, sizeof sockaddr_);
	//sockaddr_.sin_family = AF_INET;
	//sockaddr_.sin_port = htons(port);
	//int ret = inet_pton(AF_INET, host.c_str(), &sockaddr_.sin_addr);
	//valid_ = (ret == 0);
	//if (!valid_) {
	//	fprintf(stderr, "invalid address: %s %d\n", host.c_str(), port);
	//}
	struct sockaddr_in addr;
	char str_port[6];
	snprintf(str_port, 6, "%u", port);
	::bzero(&addr, sizeof addr);
	int ret = detail::resolve_addr(host, str_port, &addr);	
	set_address(addr);
	valid_ = ret;
}

InetAddress::InetAddress(const struct sockaddr_in &addr) :
		valid_(false), host_(), port_() {
	bzero(&sockaddr_, sizeof sockaddr_);
	set_address(addr);
}

bool InetAddress::set_host(const string_t &host) {
	host_ = host;
	sockaddr_.sin_family = AF_INET;
	int ret = inet_pton(AF_INET, host.c_str(), &sockaddr_.sin_addr);
	valid_ = (ret == 1);
	return valid_;
}

bool InetAddress::set_port(uint16_t port) {
	port_ = port;
	sockaddr_.sin_port = htons(port);
	return valid_;
}

bool InetAddress::set_address(struct sockaddr_in addr) {
	char buff[16] = {0};
	valid_ = false;
	port_ = ntohs(addr.sin_port);
	sockaddr_ = addr;
	const char *ret = inet_ntop(AF_INET, &addr.sin_addr, buff, sizeof buff);
	if (addr.sin_family == AF_INET && ret) {
		valid_ = true;
		host_.assign(buff);
	}
	return valid_;
}

string InetAddress::to_string() const {
	if (saddr_.empty()) {
		stringstream ss;
		ss << host() << ":" << port();
		saddr_ = ss.str();
	}
	return saddr_;
}

} // namespace net
} // namespace reactor

