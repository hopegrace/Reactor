#ifndef REACTOR_EXAMPLE_HTTP_REQUEST_H
#define REQCTOR_EXAMPLE_HTTP_REQUEST_H

namespace reactor {
namespace net {
namespace http {

class Request {
public:
	std::string method() const;
	std::string header(const string &key) const;
	std::string path() const;
	std::string url() const;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
