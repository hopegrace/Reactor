#ifndef REACTOR_NET_HTTP_RESPONSE_H
#define REACTOR_NET_HTTP_RESPONSE_H

#include <unordered_map>

#include "../TcpConnection.h"

namespace reactor {
namespace net {
namespace http {

class HttpResponse {
public:
	HttpResponse(): status_(200) 
	{
	}

	void set_status(int status);
	void set_status(int status, const std::string &text);

	void write_header(const std::string &key, const std::string &value)
	{ headers_[key] = value; }

	void write(const std::string &text)
	{ body_.append(text); }

	// for HttpServer use
	void send(const TcpConnectionPtr &conn);

private:
	typedef std::unordered_map<std::string, std::string> Header; 

	int status_;
	std::string status_text_; 
	std::string body_;
	Header headers_;
};

} // namespace http
} // namespace net
} // namespace reactor
#endif
