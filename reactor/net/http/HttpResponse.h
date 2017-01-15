#ifndef REACTOR_NET_HTTP_RESPONSE_H
#define REACTOR_NET_HTTP_RESPONSE_H

#include <unordered_map>

#include "../Buffer.h"
#include "../TcpConnection.h"

namespace reactor {
namespace net {
namespace http {

class HttpResponse {
public:
	HttpResponse(): status_(200), status_text_("OK"), body_(128)
	{
	}

	~HttpResponse() = default;
	HttpResponse(const HttpResponse &) = default;
	HttpResponse &operator=(const HttpResponse &) = default;

	void set_status(int status);
	void set_status(int status, const std::string &text);

	void write_header(const std::string &key, const std::string &value)
	{ headers_[key] = value; }

	void write(const std::string &text)
	{ body_.append(text); }

	void write(const char *data, size_t size)
	{ body_.append(data, size); }

	// for HttpServer use
	void send(const TcpConnectionPtr &conn);

private:
	typedef std::unordered_map<std::string, std::string> Header; 

	int status_;
	std::string status_text_; 
	Buffer body_;
	Header headers_;
};

} // namespace http
} // namespace net
} // namespace reactor
#endif
