#include "HttpHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace reactor {
namespace net {
namespace http {

void HttpHandler::request(const HttpRequest &request, HttpResponse *response) 
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

void HttpHandler::GET(const HttpRequest &request, HttpResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	response->set_status(200);
	response->write("OK");
}

void HttpHandler::POST(const HttpRequest &request, HttpResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	response->set_status(200);
	response->write("OK");
}

void HttpHandler::HEAD(const HttpRequest &request, HttpResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	response->set_status(200);
	response->write("OK");
}

} // namespace http
} // namespace net
} // namespace reactor
