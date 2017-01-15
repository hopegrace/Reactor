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

class Handler: public HttpHandler {
public:
	void doGet(const HttpRequest &request, HttpResponse *response) override {
		char realpath[1024];

		if (!getcwd(realpath, sizeof realpath)) {
			response->set_status(404);
			response->write("can not get cwd");
			return;
		}

		size_t len = strlen(realpath);
		if (!strncat(realpath, request.path().c_str(), sizeof realpath - len)) {
			response->set_status(404);
			response->write(request.path());
			response->write(" not exists");
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
			response->set_status(404);
			response->write(realpath);
			response->write(" not exists or not a file");
		}
	}

	void doPost(const HttpRequest &request, HttpResponse *response) override {
		doGet(request, response);
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
