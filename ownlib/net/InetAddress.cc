#include "InetAddress.h"
#include <strings.h>
#include <arpa/inet.h>

namespace sduzh {
namespace net {

typedef InetAddress::string_t string_t;

InetAddress::InetAddress(): valid_(false), host_(), port_() {
	bzero(&sockaddr_, sizeof sockaddr_);
}

InetAddress::InetAddress(const string_t &host, uint16_t port) : 
		valid_(false), host_(host), port_(port) {
	bzero(&sockaddr_, sizeof sockaddr_);
	sockaddr_.sin_family = AF_INET;
	sockaddr_.sin_port = htons(port);
	int ret = inet_pton(AF_INET, host.c_str(), &sockaddr_.sin_addr);
	valid_ = (ret == 0);
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

} // namespace net
} // namespace sduzh

