#ifndef REACTOR_NET_HTTP_REQUEST_H
#define REQCTOR_NET_HTTP_REQUEST_H

#include <string>
#include <unordered_map>

namespace reactor {
namespace net {
namespace http {

class HTTPServer;

class HTTPRequest {
public:
	friend class HTTPServer;

	typedef std::string Key;
	typedef std::string Value;
	typedef std::unordered_map<Key, Value> Header;

	HTTPRequest(): state_() {}

	std::string method() const;
	std::string header(const std::string &key) const;
	Header      headers() const;
	std::string path() const;
	std::string url() const;

private:
	int         state_;

	std::string method_;
	std::string path_;
	std::string url_;
	Header      headers_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
