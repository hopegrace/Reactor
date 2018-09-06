#include "reactor/net/http/HttpResponse.h"

namespace reactor {
namespace net {
namespace http {

void HttpResponse::set_status(int status)
{
	set_status(status, status_text(status));
}

void HttpResponse::set_status(int status, const std::string &text)
{
	status_ = status;
	status_text_ = text;
}

void HttpResponse::send(const TcpConnectionPtr &conn)
{
	char buff[20];
	status_text_ = status_text_.empty() ? status_text(status_) : status_text_;
	snprintf(buff, sizeof buff, "HTTP/1.1 %d ", status_);
	conn->write(buff);
	conn->write(status_text_);
	conn->write("\r\n");

	snprintf(buff, sizeof buff, "%lu", body_.readable_bytes());
	headers_["Content-Length"] = buff;

	for (auto & entry: headers_) {
		conn->write(entry.first);
		conn->write(": ");
		conn->write(entry.second);
		conn->write("\r\n");
	}
	conn->write("\r\n");

	conn->write(body_);
}

} // namespace http
} // namespace net
} // namespace reactor

