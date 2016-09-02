#ifndef SDUZH_TOOLS_SOCK_ADDRESS_H
#define SDUZH_TOOLS_SOCK_ADDRESS_H 

#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class InetAddress {
public:
	typedef std::string string_t;

	InetAddress();
	InetAddress(const string_t &host, uint16_t port);
	InetAddress(const struct sockaddr_in &sockaddr);

	InetAddress(const InetAddress &) = default;
	InetAddress & operator=(const InetAddress &) = default;

	bool valid() const { return valid_; }
	
	string_t host() const { return host_; }
	uint16_t port() const { return port_; }
	const struct sockaddr_in &sockaddr() const { return sockaddr_; }

	bool set_host(const string_t &host);
	bool set_port(uint16_t port);
	bool set_address(struct sockaddr_in addr);

private:
	bool valid_;

	string_t host_;
	uint16_t port_;
	struct sockaddr_in sockaddr_;
}; // class InetAddress

#endif
