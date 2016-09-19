#include <functional>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/TcpConnection.h>
#include <reactor/net/TcpServer.h>

using namespace sduzh::net;

class DiscardServer {
public:
	DiscardServer(EventLoop *loop, const InetAddress &addr):
		loop_(loop),
		server_(loop, addr) {

		using namespace std::placeholders;
		server_.set_connection_callback(std::bind(&DiscardServer::on_connection, this, _1));
		server_.set_message_callback(std::bind(&DiscardServer::on_message, this, _1));
	}

	void start() {
		server_.start();
	}

private:
	void on_connection(const TcpConnectionPtr &conn) {
		LOG(Debug) << conn->peer_address().to_string() 
				   << (conn->connected() ? "connected" : "disconnected");
		LOG(Debug) << conn->get_tcp_info_string();
	}

	void on_message(const TcpConnectionPtr &conn) {
		LOG(Debug) << "receive " << static_cast<long>(conn->message()->readable_bytes()) << " bytes";
		conn->message()->retrieve_all();
	}

	EventLoop *loop_;
	TcpServer server_;
};


int main() {
	EventLoop loop;
	DiscardServer server(&loop, InetAddress("0.0.0.0", 9090));
	server.start();

	loop.loop();
}



