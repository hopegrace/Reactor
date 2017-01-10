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

	std::string method() const { return method_; }
	const Header &headers() const { return headers_; }
	std::string path() const { return path_; }
	std::string url() const { return url_; }
	std::string header(const std::string &key) const { 
		auto it = headers_.find(key);
		return (it != headers_.end()) ? it->second : "";
	}

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
