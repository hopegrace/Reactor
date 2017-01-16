#include <fstream>
#include <string.h>
#include <unistd.h>

#include "reactor/net/EventLoop.h"
#include "reactor/net/http/HttpServer.h"
#include "reactor/net/http/HttpHandler.h"
#include "reactor/net/http/HttpRequest.h"
#include "reactor/net/http/HttpResponse.h"

using namespace std;
using namespace reactor::net;
using namespace reactor::net::http;

static const char * const kErrorMessage = \
"<!DOCTYPE html>"
"<html>"
"<head><title>Error</title><meta charset=\"utf-8\"></head>"
"<body>"
"<div style=\"text-align:center;\">"
"<h1>%d</h1>"
"<h2>%s</h2>"
"</div>"
"</body>"
"</html>";

class Handler: public HttpHandler {
public:
	void doGet(const HttpRequest &request, HttpResponse *response) override {
		char realpath[1024];

		if (!getcwd(realpath, sizeof realpath)) {
			send_error(response, 404);
			return;
		}

		size_t len = strlen(realpath);
		if (!strncat(realpath, request.path().c_str(), sizeof realpath - len)) {
			send_error(response, 404);
			return;
		}
		
		ifstream f(realpath);
		if (f) {
			// XXX: check postfix
			response->write_header("Content-type", "text/html");
			streamsize nread;
			char buff[1024];
			while ((nread=f.readsome(buff, sizeof buff)) > 0) {
				response->write(buff, nread);
			}
		} else {
			send_error(response, 404);
		}
	}

	void doPost(const HttpRequest &request, HttpResponse *response) override {
		doGet(request, response);
	}

private:
	void send_error(HttpResponse *response, int status, const char *msg=nullptr) {
		char buff[1024];
		msg = (msg == nullptr) ? status_text(status) : msg;
		snprintf(buff, sizeof buff, kErrorMessage, status, msg);
		response->set_status(status, msg);
		response->write(buff);
	}
};

int main()
{
	EventLoop loop;
	Handler handler;
	HttpServer server(&loop, InetAddress("0.0.0.0", 9000), &handler);
	server.start();
	loop.loop();
}
