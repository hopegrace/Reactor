#include <memory>

#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TcpClient.h>
#include <reactor/net/TcpServer.h>

using namespace reactor::net;

class Stream {
public:
	Stream(const TcpConnectionPtr &conn, InetAddress server_addr):
		down_stream_(conn), 
		client_(conn->loop(), server_addr) {

		using namespace std::placeholders;
		client_.set_connection_callback(std::bind(&Stream::on_server_connection, this, _1));
		client_.set_message_callback(std::bind(&Stream::on_server_message, this, _1));
		client_.connect();
	}

	~Stream() {
		assert(down_stream_->disconnected());
		client_.disconnect();
	}

	void handle_message() {
		if (client_.connection() && client_.connection()->connected()) {
			client_.connection()->write(*down_stream_->buffer());
			down_stream_->buffer()->clear();
		}
	}

	void handle_close() {
		client_.stop();
	}

private:
	void on_server_connection(const TcpConnectionPtr &conn) {
		if (conn->connected()) {
			handle_message();
		} else {
			down_stream_->close();
		}
	}

	void on_server_message(const TcpConnectionPtr &conn) {
		if (down_stream_->connected()) {
			down_stream_->write(*conn->buffer());
			conn->buffer()->clear();
		}
	}

	TcpConnectionPtr down_stream_;
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
	typedef std::shared_ptr<Stream> StreamPtr;
	typedef std::unordered_map<TcpConnectionPtr, StreamPtr> StreamList;

	void on_connection(const TcpConnectionPtr &conn) {
		LOG(Debug) << conn->peer_address().to_string() << " state " << conn->state();
		if (conn->connected()) {
			assert(streams_.find(conn) == streams_.end());
			streams_[conn] = StreamPtr(new Stream(conn, InetAddress("127.0.0.1", 9090)));
		} else {
			assert(streams_.find(conn) != streams_.end());
			streams_.erase(conn);
		}
	}

	void on_message(const TcpConnectionPtr &conn) {
		assert(streams_.find(conn) != streams_.end());
		StreamPtr stream = streams_[conn];
		stream->handle_message();
	}

	EventLoop *loop_;	
	ServerPtr server_;

	StreamList streams_;
};

int main(int argc, char *argv[]) {
	EventLoop loop;
	ProxyServer server(&loop, InetAddress("0.0.0.0", 9091));

	server.start();
	loop.loop();
	return 0;
}
