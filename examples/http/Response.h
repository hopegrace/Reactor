#ifndef REACTOR_EXAMPLE_HTTP_RESPONSE_H
#define REACTOR_EXAMPLE_HTTP_RESPONSE_H

namespace reactor {
namespace net {
namespace http {

class Response {
public:
	void set_status(int status);
	void set_status(int status, const string &msg);

	void write_header(const std::string &key, const std::string &value);
	void write(const string &text);
};

} // namespace http
} // namespace net
} // namespace reactor
#endif
