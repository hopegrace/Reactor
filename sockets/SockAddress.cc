#include "SockAddress.h"
#include <strings.h>
#include <arpa/inet.h>

typedef SockAddress::string_t string_t;

SockAddress::SockAddress(): valid_(false), host_(), port_() {
	bzero(&sockaddr_, sizeof sockaddr_);
}

SockAddress::SockAddress(const string_t &host, uint16_t port) : 
		valid_(false), host_(host), port_(port) {
	bzero(&sockaddr_, sizeof sockaddr_);
	set_host(host);
	set_port(port);
}

SockAddress::SockAddress(const struct sockaddr_in &addr) :
		valid_(false), host_(), port_() {
	bzero(&sockaddr_, sizeof sockaddr_);
	set_address(addr);
}

bool SockAddress::set_host(const string_t &host) {
	host_ = host;
	int ret = inet_pton(AF_INET, host.c_str(), &sockaddr_.sin_addr);
	valid_ = (ret == 1);
	return valid_;
}

bool SockAddress::set_port(uint16_t port) {
	port_ = port;
	sockaddr_.sin_port = htons(port);
	return valid_;
}

bool SockAddress::set_address(struct sockaddr_in addr) {
	char buff[16] = {0};
	valid_ = false;
	port_ = ntohs(addr.sin_port);
	sockaddr_ = addr;
	const char *ret = inet_ntop(AF_INET, &addr.sin_addr, buff, sizeof buff);
	host_.assign(buff);

	if (addr.sin_family == AF_INET && ret) {
		valid_ = true;
	}
	return valid_;
}

