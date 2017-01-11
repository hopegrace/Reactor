#include "HTTPResponse.h"
#include "Status.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

namespace reactor {
namespace net {
namespace http {

void HTTPResponse::set_status(int status)
{
	if (!header_end_ && status > 0) {
		status_ = status;
	}
}

void HTTPResponse::write_header(const std::string &key, const std::string &value)
{
	if (!header_end_) {
		headers_[key] = value;
	}
}

void HTTPResponse::end_headers()
{
	header_end_ = true;
}

void HTTPResponse::write(const std::string &text)
{
	if (status_) {
		send_headers();
		status_ = 0; // 防止再次发送
	}
	conn_->write(text);
}

void HTTPResponse::send_headers()
{
	assert(status_ > 0);
	/* response line */
	conn_->write("HTTP/1.1 ");
	conn_->write(status_);
	conn_->write(" ");
	conn_->write(status_text(status_));
	conn_->write(CRLF);

	/* respose headers */
	for (auto it = headers_.begin(); it != headers_.end(); ++it) {
		conn_->write(it->first);
		conn_->write(": ");
		conn_->write(it->second);
		conn_->write(CRLF);
	}
	conn_->write(CRLF);
}

} // namespace http
} // namespace net
} // namespace reactor

