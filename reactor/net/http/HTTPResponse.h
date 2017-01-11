#ifndef REACTOR_NET_HTTP_RESPONSE_H
#define REACTOR_NET_HTTP_RESPONSE_H

#include <unordered_map>

#include "../TcpConnection.h"

namespace reactor {
namespace net {
namespace http {

class HTTPResponse {
public:
	HTTPResponse(const TcpConnectionPtr &conn): conn_(conn) { /**/ }
	~HTTPResponse() = default;

	HTTPResponse(const HTTPResponse &) = delete;
	HTTPResponse & operator = (const HTTPResponse &) = delete;

	void set_status(int status);

	void write_header(const std::string &key, const std::string &value);
	void end_headers();
	void write(const std::string &text);

private:
	typedef std::string Key;
	typedef std::string Val;
	typedef std::unordered_map<Key, Val> Header; 

	void send_headers();

	TcpConnectionPtr conn_;

	int  status_ = 200;
	bool header_end_ = false;
	Header headers_;
};

} // namespace http
} // namespace net
} // namespace reactor
#endif
