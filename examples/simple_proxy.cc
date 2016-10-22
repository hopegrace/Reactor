#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <libgen.h> // basename

#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/TcpClient.h>
#include <reactor/net/TcpServer.h>

using namespace reactor::net;

class Stream {
public:
	Stream(const TcpConnectionPtr &conn, InetAddress server_addr):
		client_(conn->loop(), server_addr),
		down_stream_(conn), 
		up_stream_() {

		using namespace std::placeholders;
		client_.set_connection_callback(std::bind(&Stream::up_stream_connection, this, _1));
		client_.set_message_callback(std::bind(&Stream::up_stream_message, this, _1));
		client_.connect();
	}

	void handle_down_stream_message() {
		if (up_stream_) {
			assert(up_stream_->connected());
			up_stream_->write(down_stream_->buffer());
			down_stream_->buffer()->clear();
		} else {
			// TODO 
		}
	}

private:
	void up_stream_connection(const TcpConnectionPtr &conn) {
		if (conn->connected()) {
			assert(!up_stream_);
			up_stream_ = conn;
			handle_down_stream_message();
		} else {
			assert(conn == up_stream_);
			down_stream_->close();
		}
	}

	void up_stream_message(const TcpConnectionPtr &conn) {
		assert(conn == up_stream_); (void)conn;
		assert(down_stream_->connected());
		down_stream_->write(up_stream_->buffer());
		up_stream_->buffer()->clear();
	}

	TcpClient client_;
	TcpConnectionPtr down_stream_;
	TcpConnectionPtr up_stream_;
};

class ProxyServer {
public:
	ProxyServer(EventLoop *loop, InetAddress addr): 
		loop_(loop), 
		server_(new TcpServer(loop, InetAddress("0.0.0.0", 9091))),
		remote_addr_(addr),
		streams_() {

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
			streams_[conn] = StreamPtr(new Stream(conn, remote_addr_));
		} else {
			assert(streams_.find(conn) != streams_.end());
			streams_.erase(conn);
		}
	}

	void on_message(const TcpConnectionPtr &conn) {
		assert(streams_.find(conn) != streams_.end());
		StreamPtr stream = streams_[conn];
		stream->handle_down_stream_message();
	}

	EventLoop *loop_;	
	ServerPtr server_;
	InetAddress remote_addr_;

	StreamList streams_;
};

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: %s <remote host> <remote port>\n", basename(argv[0]));
		return 0;
	}

	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

	EventLoop loop;
	ProxyServer server(&loop, InetAddress(argv[1], port));
	LOG(Info) << "proxy server binding on 0.0.0.0:9091";
	server.start();
	loop.loop();
	return 0;
}
