#include <memory>

#include <reactor/net/EventLoop.h>
#include <reactor/net/TcpClient.h>
#include <reactor/net/TcpServer.h>

using namespace reactor::net;

class Stream {
public:
	Stream(const TcpConnectionPtr &conn):
		conn_(conn), 
		client_() {
	}
	
private:
	TcpConnectionPtr conn_;
	TcpClient client_;
};

class ProxyServer {
public:
	ProxyServer(EventLoop *loop, InetAddress bind_addr): 
		loop_(loop), server_(new TcpServer(loop, bind_addr)) {

		using namespace std::placeholders;
		server_->set_connection_callback(std::bind(&ProxyServer::on_connection, this, _1));
		server_->set_message_callback(std::bind(&ProxyServer::on_message, this, _1));
	}

	/// disable copy-ctor and copy-assign
	ProxyServer(const ProxyServer &) = delete;
	ProxyServer &operator=(const ProxyServer &) = delete;

	void start() {
		server_->start();
	}

private:
	typedef std::unique_ptr<TcpServer> ServerPtr;
	typedef std::unordered_map<int, TcpConnectionPtr> ClientList;

	void on_connection(const TcpConnectionPtr &conn) {
		if (conn->connected()) {
			assert(clients_.find(conn->fd()) == clients_.end());
			clients_[conn->fd()] = conn;
		} else {
			assert(clients_.find(conn->fd()) != clients_.end());
			clients_.erase(conn->fd());
		}
	}

	void on_message(const TcpConnectionPtr &conn) {
	}

	EventLoop *loop_;	
	ServerPtr server_;

	ClientList clients_;
};

int main(void) {
	EventLoop loop;
	ProxyServer server(&loop, InetAddress("0.0.0.0", 9091));

	server.start();
	loop.loop();
	return 0;
}
