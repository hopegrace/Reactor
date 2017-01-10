#ifndef REACTOR_NET_HTTP_RESPONSE_H
#define REACTOR_NET_HTTP_RESPONSE_H

namespace reactor {
namespace net {
namespace http {

class HTTPResponse {
public:
	void set_status(int status);
	void set_status(int status, const std::string &msg);

	void write_header(const std::string &key, const std::string &value);
	void write(const std::string &text);
};

} // namespace http
} // namespace net
} // namespace reactor
#endif
