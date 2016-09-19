#include <reactor/net/TcpConnection.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/SocketOpt.h>

using namespace std;
using namespace sduzh::base;

namespace sduzh {
namespace net {

static void default_connection_callback(const TcpConnectionPtr &conn) {
	if (conn->connected()) {
		LOG(Info) << conn->peer_address().to_string() << " connected\n";
	} else {
		LOG(Info) << conn->peer_address().to_string() << " disconnected\n";
	}
}

static void default_message_callback(const TcpConnectionPtr &conn) {
	conn->message()->clear();
}

TcpConnection::TcpConnection(EventLoop *loop, int fd):
		std::enable_shared_from_this<TcpConnection>(),
		loop_(loop),
		socket_(fd),
		channel_(loop, fd),
		state_(kConnecting),
		connection_cb_(default_connection_callback),
		message_cb_(default_message_callback),
		write_complete_cb_(),
		close_cb_(),
		input_buffer_(1024),
		output_buffer_(1024),
		local_addr_(socket_.getsockname()), 
		peer_addr_(socket_.getpeername()) {
	socket_.set_blocking(false);
	channel_.set_read_callback(std::bind(&TcpConnection::on_read, this));
	channel_.set_write_callback(std::bind(&TcpConnection::on_write, this));
	channel_.set_close_callback(std::bind(&TcpConnection::on_close, this));
	channel_.set_error_callback(std::bind(&TcpConnection::on_error, this));
}

TcpConnection::~TcpConnection() {
}

void TcpConnection::abort() {
	if (state_ == kConnected || state_ == kDisconnecting) {
		on_close();
	}
}

void TcpConnection::close() {
	if (state_ == kConnected) {
		set_state(kDisconnecting);
		if (output_buffer_.readable_bytes() == 0) { // no data to write
			on_close();
		} 
	}
}

bool TcpConnection::get_tcp_info(struct tcp_info *tcpi) const {
	return socket_.get_tcp_info(tcpi);
}

string TcpConnection::get_tcp_info_string() const {
	char buffer[1024];
	buffer[0] = '\0';
	socket_.get_tcp_info_string(buffer, sizeof buffer);
	return buffer;
}

// TODO more efficiency
void TcpConnection::write(const char *buffer, size_t len) {
	if (state_ == kConnected) {
		if (!output_buffer_.empty()) {
		    assert(channel_.events() & EVENT_WRITE);
			// TODO limit size
		    output_buffer_.append(buffer, len);
		} else {
		    assert((channel_.events() & EVENT_WRITE) == 0);
		    ssize_t nsend = socket_.send(buffer, len);
		    if (nsend < 0) {
				LOG(Error) << strerror(errno);
		    } else if (nsend < static_cast<int>(len)) {
				buffer += nsend;
				len -= nsend;
		    	output_buffer_.append(buffer, len);
		    	channel_.enable_write();
		    } else {  // nsend == len
		    	if (write_complete_cb_) {
		    		write_complete_cb_(shared_from_this());
		    	}
		    }
		}
	} else {
		LOG(Debug) << "not connected, write ignored";
	}
}

void TcpConnection::on_read() {
	// TODO use Buffer
	int nread = input_buffer_.read_fd(socket_.fd());
	if (nread < 0) {
		LOG(Error) << strerror(errno);
	} else if (nread == 0) {
		on_close();
	} else {
		message_cb_(shared_from_this());
	}
}

// TODO more efficiency
void TcpConnection::on_write() {
	assert(state_ == kConnected || state_ == kDisconnecting);
	assert(output_buffer_.readable_bytes());
	ssize_t nsend = socket_.send(output_buffer_.data(), output_buffer_.readable_bytes());
	if (nsend > 0) {
		output_buffer_.retrieve(nsend);
		if (output_buffer_.empty()) {
			channel_.disable_write();
			if (write_complete_cb_) { 
				write_complete_cb_(shared_from_this()); 
			}
			if (state_ == kDisconnecting) { 
				on_close(); 
			}
		} 	
	} else {
		LOG(Error) << strerror(errno);
	}
}

void TcpConnection::on_close() {
	assert(state_ == kConnected || state_ == kDisconnecting);
	set_state(kDisconnected);
	channel_.disable_all();
	connection_cb_(shared_from_this());
	channel_.remove();
	// socket_.close();
	if (close_cb_) {
		close_cb_(shared_from_this());
	}
}

void TcpConnection::on_error() {
	// TODO 
	LOG(Info) << "not implement";
}

void TcpConnection::connection_established() {
	if (state_ == kConnecting) {
		set_state(kConnected);
		connection_cb_(shared_from_this()); 
	}
}

void TcpConnection::set_tcp_nodelay(bool on) {
	int r = sockets::set_tcp_nodelay(socket_.fd(), on);
	if (r < 0) {
		LOG(Error) << strerror(errno);
	}
}

} // namespace net
} // namespace sduzh

