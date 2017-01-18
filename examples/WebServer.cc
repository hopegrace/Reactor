#include <algorithm>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "reactor/base/os.h"
#include "reactor/base/path.h"
#include "reactor/base/SimpleLogger.h"
#include "reactor/base/strings/strings.h"
#include "reactor/net/EventLoop.h"
#include "reactor/net/http/HttpServer.h"
#include "reactor/net/http/HttpHandler.h"
#include "reactor/net/http/HttpRequest.h"
#include "reactor/net/http/HttpResponse.h"

using namespace std;
using namespace reactor::base;
using namespace reactor::base::strings;
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

class WebServer: public HttpHandler {
public:
	WebServer(EventLoop *loop, InetAddress addr):
		server_(loop, addr, this) 
	{
	}

	~WebServer() = default;
	WebServer(const WebServer &) = delete;
	WebServer &operator=(const WebServer &) = delete;

	void start() {
		server_.start();
	}

	void doGet(const HttpRequest &request, HttpResponse *response) override {
		LOG(Debug) << "request path: " << request.path();
		string reqpath = request.path().empty() ? "/index.html" : request.path();
		string abspath = path::abspath(path::join(".", reqpath.substr(1)));
		if (!startswith(abspath, path::abspath("."))) {
			send_error(response, 404);

		} else if (path::isdir(abspath)) {
			char buff[1000];
			vector<string> names = os::listdir(abspath);
			sort(names.begin(), names.end());
			response->write_header("Content-type", "text/html");
			response->write("<!DOCTYPE html><html><head><title>list dir</title>");
			response->write("<meta charste=\"utf-8\"></head><body>");
			for (const auto &name: names) {
				snprintf(buff, sizeof buff, "<p><a href=%s>%s</a></p>", 
						path::join(reqpath, name).c_str(), name.c_str());
				response->write(buff);
			}
			response->write("</body></html>");

		} else {
			FILE *f = fopen(abspath.c_str(), "r");
			if (f) {
				response->write_header("Content-type", "text/plain");
				char data[1024];
				size_t nread = 0;
				while ((nread=fread(data, 1, sizeof data, f)) > 0) {
					response->write(data, nread);
				}
				fclose(f);
			} else {
				send_error(response, 500, strerror(errno));
			}
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

	HttpServer server_;
};

int main()
{
	EventLoop loop;
	WebServer server(&loop, InetAddress("0.0.0.0", 9000));
	server.start();
	loop.loop();
}
