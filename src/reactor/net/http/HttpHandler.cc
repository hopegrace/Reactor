#include "reactor/net/http/HttpHandler.h"
#include "reactor/net/http/HttpRequest.h"
#include "reactor/net/http/HttpResponse.h"

namespace reactor {
namespace net {
namespace http {

void HttpHandler::request(const HttpRequest &request, HttpResponse *response) 
{
	if (request.method() == "GET") {
		doGet(request, response);
	} else if (request.method() == "POST") {
		doPost(request, response);
	} else if (request.method() == "HEAD") {
		doHead(request, response);
	}else {
		response->set_status(405);
	}
}

void HttpHandler::doGet(const HttpRequest &request, HttpResponse *response)
{
	printf("%s %s %s\n", request.method().c_str(), request.url().c_str(), request.version().c_str());
	printf("path  : %s\n", request.path().c_str());
	printf("query : %s\n", request.query().c_str());
	printf("headers:\n");
	for (const auto &entry: request.headers()) {
		printf("\t%s: %s\n", entry.first.c_str(), entry.second.c_str());
	}
	response->write("OK");
}

void HttpHandler::doPost(const HttpRequest &request, HttpResponse *response)
{
	doGet(request, response);
}

void HttpHandler::doHead(const HttpRequest &request, HttpResponse *response)
{
	doGet(request, response);
}

} // namespace http
} // namespace net
} // namespace reactor
