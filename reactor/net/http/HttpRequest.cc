#include "HttpRequest.h"
#include <assert.h>
#include <string.h>

#include "reactor/base/SimpleLogger.h"

#define ERROR_RETURN(e) set_error(e); return

namespace reactor {
namespace net {
namespace http {

void HttpRequest::parse()
{
	char *data = conn_->buffer()->as_str();
	char *line = data;
	char *crlf = nullptr;

	while (!finished() && !error() && (crlf=strstr(line, "\r\n")) != nullptr) {
		*crlf = '\0';

		switch (state_) {
		case kInit:
			parse_method(line);
			break;
		case kHeader:
			parse_header(line);
			break;
		default:
			printf("unknown state %d\n", state_);
		}

		line = crlf + 2; // move to next line
	}

	// TODO FORM data; 

	conn_->buffer()->retrieve(static_cast<int>(line - data));
}

void HttpRequest::parse_method(char *line)
{
	char method[1024], url[1024], version[1024];
	assert(state_ == kInit);

	if (strlen(line) >= sizeof method) {
		ERROR_RETURN(1); 
	}

	if (sscanf(line, "%[^ ] %[^ ] %[^ ]", method, url, version) != 3) {
		ERROR_RETURN(1);
	}

	url_ = url;
	method_ = method;
	version_ = version;

	if (url_[0] != '/') {
		ERROR_RETURN(1);
	}
	if (version_ != "HTTP/1.1") {
		ERROR_RETURN(1);
	}

	set_state(kHeader);
}

void HttpRequest::parse_header(char *line)
{
	char key[1024], value[1024];
	size_t len = strlen(line);

	if (len >= sizeof(key)) {
		set_error(1);
	} else if (len == 0) {
		set_state(kFinish);
	} else if (sscanf(line, "%[^:]:%s", key, value) == 2) {
		headers_[key] = value;
	} else {
		set_error(1);
	}
}

} // namespace http
} // namespace net
} // namespace reactor