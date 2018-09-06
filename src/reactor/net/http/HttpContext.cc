#include "reactor/net/http/HttpContext.h"

#include "reactor/base/strings/strings.h"

using namespace std;
using namespace reactor::base;

namespace reactor {
namespace net {
namespace http {

void HttpContext::parse(const TcpConnectionPtr &conn)
{
	char *data = conn->buffer()->as_str();
	char *line = data;
	char *crlf = nullptr;

	while (!finished() && !error() && (crlf=strstr(line, "\r\n")) != nullptr) {
		*crlf = '\0';

		switch (state_) {
		case kRequestLine:
			parse_request_line(line);
			break;
		case kHeader:
			parse_header(line);
			break;
		case kBody:
			// TODO
			break;
		default:
			printf("unknown state %d\n", state_);
		}

		line = crlf + 2; // move to next line
	}

	std::string value = strings::lower(request_.header("Connection"));
	if (value == "close") {
		close_connection_ = true;
	} else if (value == "keep-alive" && request_.version() >= "HTTP/1.1") {
		close_connection_ = false;
	}

	conn->buffer()->retrieve(static_cast<int>(line - data));
}

void HttpContext::parse_request_line(const std::string &line)
{
	string command, url, version;
	assert(state_ == kRequestLine);

	if (line.size() >= 65535) {
		set_error(1, "invalid request");
		return;
	}

	vector<string> words = strings::split(line, ' ');
	if (words.size() == 3) {
		command = words[0]; url = words[1]; version = words[2];

		if (version.substr(0, 5) != "HTTP/") {
			set_error(400, "Bad request version " + version);
			return;
		}
		vector<string> version_numbers = strings::split(version.substr(5), '.');
		if (version_numbers.size() != 2) {
			set_error(400, "Bad request version " + version);
			return;
		}
		unsigned v1, v2;
		bool     b1, b2;
		v1 = strings::to_uint(version_numbers[0], &b1);
		v2 = strings::to_uint(version_numbers[1], &b2);
		if (!b1 || !b2) {
			set_error(400, "Bad request version " + version);
			return;
		}
		if (v1 >= 2) {
			set_error(505, "Invalid HTTP Version " + version);
			return;
		}
		if (v2 >= 1 && v2 >= 1) {
			close_connection_ = false;
		}


	} else if (words.size() == 2) {
		command = words[0]; url = words[1];
		close_connection_ = true;
		if (command != "GET") {
			set_error(400, "Bad HTTP/0.9 request type " + command);
			return;
		}
	} else {
		set_error(400, "Bad request syntax " + line);
		return;
	}

	request_.set_method(command);
	request_.set_url(url);
	request_.set_version(version);

	size_t question = url.find('?');
	if (question != string::npos) {
		request_.set_path(url.substr(0, question));
		request_.set_query(url.substr(question+1));
	} else {
		request_.set_path(url);	
	}

	state_ = kHeader;
}

void HttpContext::parse_header(const std::string &line)
{
	if (line.size() >= 65535) {
		set_error(400, "Bad request header (too long)");
		return;
	} 

	if (line.empty()) {
		state_ = kFinish;
		return;
	}

	size_t colon = line.find(':');
	if ((colon != string::npos) && (colon != (line.size() - 1))) {
		request_.set_header(line.substr(0, colon), line.substr(colon+1));
	} else {
		set_error(400, "Bad request header " + line);
	}
}

void HttpContext::set_error(int err, const std::string &msg) 
{
	error_code_ = err;
	error_msg_ = msg;
}

} // namespace http
} // namespace net
} // namespace reactor
