#include "HTTPServer.h"
#include <iostream>
#include "../TcpConnection.h"
#include "../../base/SimpleLogger.h"
#include "HTTPHandler.h"
#include "HTTPResponse.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif 

using namespace std;

namespace reactor {
namespace net {
namespace http {

void HTTPServer::on_connection(const TcpConnectionPtr &conn) {
	bool connected = conn->connected();
	LOG(Debug) << conn->peer_address().to_string() << (connected ? " connected" : " disconnected");
	if (connected) {
		assert(clients_.find(conn) == clients_.end());
		clients_.insert(std::make_pair(conn, HTTPRequest()));
	} else {
		assert(clients_.find(conn) != clients_.end());
		clients_.erase(conn);
	}
}

void HTTPServer::on_message(const TcpConnectionPtr &conn) {
	int err = 0;
	auto it = clients_.find(conn);
	assert(it != clients_.end());
	HTTPRequest &request = it->second;

	LOG(Debug) << "message:\n" << conn->buffer()->as_str();

	while (!err) {
		switch (request.state_) {
		case kInit:
			err = request_line(conn->buffer(), &request);
			break;
		case kInHeader:
			err = request_header(conn->buffer(), &request);
		case kFinish:
			if (handler_) {
				HTTPResponse response;
				if (request.method() == "GET") {
					handler_->GET(request, &response);
				} else if (request.method() == "POST") {
					handler_->POST(request, &response);
				}
			}
			err = 1; // clonse connection
		default:
			break;
		}
	}

	if (err) {
		conn->close();
	}
}

int HTTPServer::request_line(reactor::net::Buffer *buff, HTTPRequest *request)
{
	char *data = buff->as_str();
	char *crlf = strstr(data, "\r\n");
	char method[1024], url[1024], version[1024];
	if (!crlf) {
		return 0;
	}
	
	if (crlf - data >= 1024) {
		// TODO invalid HTTP request
		goto error;
	}
	crlf[0] = '\0';

	if (sscanf(data, "%[^ ] %[^ ] %[^ ]", method, url, version) != 3) {
		// TODO invalid HTTP request
		goto error;
	}

	if (strcmp(method, "GET") && strcmp(method, "POST")) {
		// TODO method not supported
		goto error;
	}

	if (strcmp(version, "HTTP/1.1")) {
		// TODO version not supported
		goto error;
	}

	request->method_ = method;
	request->url_ = url;
	request->state_ = kInHeader;

	buff->retrieve(crlf + 2 - data);
	return 0;	

error:
	return 1;
}

int HTTPServer::request_header(reactor::net::Buffer *buff, HTTPRequest *request)
{
	char *line = buff->as_str();
	char *crlf = 0;
	char key[1024], value[1024];
	while ((crlf=strstr(line, "\r\n")) != nullptr) {
		*crlf = '\0';
		if (crlf == line) {
			line = crlf + 2;
			break;
		}

		if (crlf - line >= 1024) {
			return 1;
		}

		if (sscanf(line, "%[^:]:%s", key, value) != 2) {
			LOG(Error) << "invalid header";
			return 1;
		}

		request->headers_[key] = value;

		line = crlf + 2;
	}

	// TODO 
	request->state_ = kFinish; 
	buff->retrieve(line - buff->data());
	return 0;
}

int HTTPServer::request_body(reactor::net::Buffer *buff, HTTPRequest *request)
{
	return 0;
}


} // namespace http
} // namespace net
} // namespace reactor
