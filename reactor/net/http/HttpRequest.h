#ifndef REACTOR_NET_HTTP_REQUEST_H
#define REQCTOR_NET_HTTP_REQUEST_H

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

	// constructor
	explicit HttpRequest(const TcpConnectionPtr &conn): 
		conn_(conn) 
	{
	}

	~HttpRequest() = default;

	HttpRequest(const HttpRequest &) = default;
	HttpRequest & operator=(const HttpRequest &) = default;

	std::string method() const { return method_; }
	const Header &headers() const { return headers_; }
	std::string path() const { return path_; }
	std::string url() const { return url_; }
	std::string version() const { return version_; }
	std::string header(const std::string &key) const { 
		auto it = headers_.find(key);
		return (it != headers_.end()) ? it->second : "";
	}
	
private:
	enum State {
		kInit = 0,
		kHeader,
		kBody,
		kFinish,
		kError,
	};

	// 返回处理的字节数
	void parse();

	bool finished() { 
		return state_ == kFinish; 
	}

	int error() { 
		return error_;
	}

	void set_error(int e) { 
		error_ = e; 
		state_ = kError; 
	}

	void set_state(State stat) {
		state_ = stat;
	}

	void parse_method(char *line);
	void parse_header(char *line);

	TcpConnectionPtr conn_;

	State state_ = kInit;
	int   error_ = 0;

	std::string method_;
	std::string path_;
	std::string url_;
	std::string version_;
	Header      headers_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
