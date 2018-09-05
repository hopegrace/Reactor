#ifndef REACTOR_NET_HTTP_REQUEST_H
#define REACTOR_NET_HTTP_REQUEST_H

#include <string>
#include <unordered_map>

#include "../TcpConnection.h"

namespace reactor {
namespace net {
namespace http {

class HttpServer;

class HttpRequest {
public:
	friend class HttpServer;

	typedef std::string Key;
	typedef std::string Value;
	typedef std::unordered_map<Key, Value> Header;

	// HttpRequest() = default;
	// ~HttpRequest() = default;

	// HttpRequest(const HttpRequest &) = default;
	// HttpRequest & operator=(const HttpRequest &) = default;

	//
	// getters
	//

	std::string method() const 
	{ return method_; }

	const Header &headers() const 
	{ return headers_; }

	std::string url() const 
	{ return url_; }

	std::string path() const 
	{ return path_; }

	std::string query() const 
	{ return query_; }

	std::string version() const 
	{ return version_; }

	std::string header(const std::string &key) const { 
		auto it = headers_.find(key);
		return (it != headers_.end()) ? it->second : "";
	}

	//
	// setters
	//
	
	void set_method(const std::string &method)
	{ method_ = method; }

	void set_header(const std::string &field, const std::string &value)
	{ headers_[field] = value; }

	void set_url(const std::string &url) 
	{ url_ = url; }

	void set_path(const std::string &path) 
	{ path_ = path; }

	void set_query(const std::string &query) 
	{ query_ = query; }

	void set_version(const std::string &version)
	{ version_ = version; }
	
private:
	std::string method_;
	std::string url_;
	std::string path_;
	std::string query_;
	std::string version_;
	Header      headers_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
