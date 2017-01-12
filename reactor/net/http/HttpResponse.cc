#include "HttpResponse.h"
#include "Status.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

namespace reactor {
namespace net {
namespace http {

void HttpResponse::set_status(int status)
{
	if (!header_end_ && status > 0) {
		status_ = status;
	}
}

void HttpResponse::write_header(const std::string &key, const std::string &value)
{
	if (!header_end_) {
		headers_[key] = value;
	}
}

void HttpResponse::end_headers()
{
	header_end_ = true;
}

void HttpResponse::write(const std::string &text)
{
	if (status_) {
		send_headers();
		status_ = 0; // 防止再次发送
	}
	conn_->write(text);
}

void HttpResponse::send_headers()
{
	assert(status_ > 0);

	char line[1024];

	snprintf(line, sizeof line, "HTTP/1.1 %d %s\r\n", status_, status_text(status_).c_str());
	conn_->write(line);

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

