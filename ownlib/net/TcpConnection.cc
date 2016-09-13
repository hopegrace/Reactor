#include <ownlib/net/TcpConnection.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ownlib/base/SimpleLogger.h>

using namespace sduzh::base;

namespace sduzh {
namespace net {

static void default_connection_callback(TcpConnection *conn) {
	if (conn->connected()) {
		LOG(Info) << conn->peer_address().to_string() << " connected\n";
	} else {
		LOG(Info) << conn->peer_address().to_string() << " disconnected\n";
	}
}

static void default_message_callback(TcpConnection *conn) {
	conn->message()->clear();
}

TcpConnection::TcpConnection(EventLoop *loop, int fd):
		loop_(loop),
		socket_(fd),
		channel_(loop, fd),
		state_(kConnecting),
		connection_cb_(default_connection_callback),
		message_cb_(default_message_callback),
		write_complete_cb_(),
		close_cb_(),
		read_buffer_(),
		write_buffer_(),
		local_addr_(socket_.getsockname()), 
		peer_addr_(socket_.getpeername()) {
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
		if (write_buffer_.empty()) { // no data to write
			on_close();
		} 
	}
}

void TcpConnection::write(const char *buffer, size_t len) {
	if (state_ == kConnected) {
		if (!write_buffer_.empty()) {
		    assert(channel_.events() & EVENT_WRITE);
			// TODO limit size
		    write_buffer_.insert(write_buffer_.end(), buffer, buffer + len);
		} else {
			assert(state_ == kConnected);
		    assert((channel_.events() & EVENT_WRITE) == 0);
		    ssize_t nsend = socket_.send(buffer, len);
		    if (nsend < 0) {
				LOG(Error) << strerror(errno);
		    } else if (nsend < static_cast<int>(len)) {
				buffer += nsend;
				len -= nsend;
		    	write_buffer_.insert(write_buffer_.end(), buffer, buffer + len);
		    	channel_.enable_write();
		    } else {  // nsend == len
		    	if (write_complete_cb_) {
		    		write_complete_cb_(this);
		    	}
		    }
		}
	} else {
		LOG(Debug) << "not connected, write ignored";
	}
}

void TcpConnection::on_read() {
	// TODO use Buffer
	char buffer[1024];
	ssize_t nread = socket_.recv(buffer, sizeof buffer);
	if (nread < 0) {
		LOG(Error) << strerror(errno);
	} else if (nread == 0) {
		on_close();
	} else {
		read_buffer_.insert(read_buffer_.end(), buffer, buffer + nread);
		message_cb_(this);
	}
}

// TODO more efficient
void TcpConnection::on_write() {
	assert(state_ == kConnected || state_ == kDisconnecting);
	assert(write_buffer_.size());
	ssize_t nsend = socket_.send(write_buffer_.data(), write_buffer_.size());
	if (nsend < 0) {
		LOG(Error) << strerror(errno);
	} else {
		if (nsend == static_cast<ssize_t>(write_buffer_.size())) {
			write_buffer_.clear();
			channel_.disable_write();
			if (write_complete_cb_) { 
				write_complete_cb_(this); 
			}
			if (state_ == kDisconnecting) { 
				on_close(); 
			}
		} else {
			auto begin = write_buffer_.begin();
			write_buffer_.erase(begin, begin + nsend);
		}
	}
}

void TcpConnection::on_close() {
	assert(state_ == kConnected || state_ == kDisconnecting);
	set_state(kDisconnected);
	channel_.disable_all();
	connection_cb_(this);
	channel_.remove();
	// socket_.close();
	if (close_cb_) {
		// may delete 'this'
		close_cb_(this);
	}
}

void TcpConnection::on_error() {
	// TODO 
}

void TcpConnection::connection_established() {
	if (state_ == kConnecting) {
		set_state(kConnected);
		connection_cb_(this); 
	}
}

} // namespace net
} // namespace sduzh
