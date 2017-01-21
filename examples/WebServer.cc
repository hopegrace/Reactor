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
#include "reactor/net/http/mimetypes.h"

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
		mimetypes::add_type("text/plain", ".c");
		mimetypes::add_type("text/plain", ".cc");
		mimetypes::add_type("text/plain", ".h");
		mimetypes::add_type("text/plain", ".py");
	}

	~WebServer() = default;
	WebServer(const WebServer &) = delete;
	WebServer &operator=(const WebServer &) = delete;

	void start() 
	{ server_.start(); }

	void doGet(const HttpRequest &request, HttpResponse *response) override {
		LOG(Debug) << "request path: " << request.path();
		string reqpath = request.path().empty() ? "index.html" : request.path().substr(1);
		string abspath = path::abspath(path::join(".", reqpath));
		if (!startswith(abspath, path::abspath(".")) || !path::exists(abspath)) {
			send_error(response, 404);

		} else if (path::isdir(abspath)) {
			char buff[1000];
			vector<string> names = os::listdir(abspath);
			transform(names.begin(), names.end(), names.begin(), 
				[&reqpath](const string &path) -> string {
					return path::isdir(path::join(reqpath, path)) ? path + "/" : path;
				});

			sort(names.begin(), names.end(), 
				[&reqpath](const string &path_1, const string &path_2) -> bool {
					bool b1 = (path_1.back() == '/');
					bool b2 = (path_2.back() == '/');
					return (b1 != b2) ? b1 : path_1 < path_2;
				 });

			response->write_header("Content-type", "text/html");
			response->write("<!DOCTYPE html><html><head><title>list dir</title>");
			response->write("<meta charste=\"utf-8\"></head><body>");
			for (const auto &name: names) {
				snprintf(buff, sizeof buff, "<p><a href=/%s>%s</a></p>", 
						path::join(reqpath, name).c_str(), name.c_str());
				response->write(buff);
			}
			response->write("</body></html>");

		} else {
			FILE *f = fopen(abspath.c_str(), "r");
			if (f) {
				string type;
				string encoding;
				guess_type(abspath, &type, &encoding);
				response->write_header("Content-Type", type);
				if (encoding.size()) {
					response->write_header("Content-Encoding", encoding);
				}
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

	void guess_type(const std::string &path, std::string *type, std::string *encoding) {
		mimetypes::guess_type(path, type, encoding);
		if (type->empty()) {
			*type = "application/octet-stream";
		}
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
