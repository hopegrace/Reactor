#include "HTTPHandler.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace reactor {
namespace net {
namespace http {

void HTTPHandler::request(const HTTPRequest &request, HTTPResponse *response) 
{
	if (request.method() == "GET") {
		GET(request, response);
	} else if (request.method() == "POST") {
		POST(request, response);
	} else {
		response->set_status(405);
	}

	response->write(""); // send data
}

void HTTPHandler::GET(const HTTPRequest &request, HTTPResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	response->set_status(200);
	response->write("OK");
}

void HTTPHandler::POST(const HTTPRequest &request, HTTPResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	response->set_status(200);
	response->write("OK");
}

void HTTPHandler::HEAD(const HTTPRequest &request, HTTPResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	response->set_status(200);
	response->write("OK");
}

} // namespace http
} // namespace net
} // namespace reactor
