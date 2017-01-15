#ifndef REACTOR_NET_HTTP_CONTEXT_H
#define REACTOR_NET_HTTP_CONTEXT_H

#include "../TcpConnection.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace reactor {
namespace net {
namespace http {

class HttpRequest;
class HttpResponse;

class HttpContext {
public:
	enum State {
		kRequestLine = 0,
		kHeader,
		kBody,
		kFinish,
		kError,
	};


	HttpContext():
		state_(kRequestLine),
		error_code_(200),
		error_msg_("OK"),
		close_connection_(true) 
	{
	}

	~HttpContext() = default;
	HttpContext(const HttpContext &) = default;
	HttpContext &operator=(const HttpContext &) = default;

	void reset() {
		request_ = HttpRequest();
		response_ = HttpResponse();
		state_ = kRequestLine;
		error_code_ = 200;
		error_msg_ = "OK";
		close_connection_ = true;
	}

	void parse(const TcpConnectionPtr &conn);

	bool close_connection() const 
	{ return close_connection_; }

	bool finished() const 
	{ return state_ == kFinish; }

	bool error() const 
	{ return error_code_ != 200; }

	int error_code() const
	{ return error_code_; }

	const std::string &error_message() const 
	{ return error_msg_; }

	HttpRequest &request() 
	{ return request_; }

	const HttpRequest &request() const
	{ return request_; }

	HttpResponse &response()
	{ return response_; }

	const HttpResponse &response() const
	{ return response_; }

private:
	void parse_request_line(const std::string &line);
	void parse_header(const std::string &line);

	void set_error(int err, const std::string &msg);

	State state_;

	int error_code_;
	std::string error_msg_;

	bool close_connection_;

	HttpRequest request_;
	HttpResponse response_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
