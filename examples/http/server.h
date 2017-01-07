#ifndef REACTOR_EXAMPLE_HTTP_SERVER_H
#define REACTOR_EXAMPLE_HTTP_SERVER_H

namespace reactor {
namespace net {
namespace http {

class Server {
public:
	Server(InetAddress addr, uint16_t port);

	void start();

private:
	void on_connection	(const TcpConnectionPtr &conn);
	void on_message		(const TcpConnectionPtr &conn);

	TcpServer server_;
	BaseHandler *handler_;
};

} // namespace http
} // namespace net
} // namespace reactor

#endif
