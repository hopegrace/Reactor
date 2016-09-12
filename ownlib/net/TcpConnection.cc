#include <ownlib/net/TcpConnection.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ownlib/base/SimpleLogger.h>

using namespace sduzh::base;

namespace sduzh {
namespace net {

TcpConnection::TcpConnection(EventLoop *loop, int fd):
		loop_(loop),
		socket_(fd),
		channel_(loop, fd),
		connected_(false),
		connection_cb_(),
		message_cb_(),
		write_complete_cb_(),
		close_cb_(),
		read_buffer_(),
		write_buffer_() {
	using namespace std::placeholders;
	channel_.set_read_callback(std::bind(&TcpConnection::on_read, this, _1));
	channel_.set_write_callback(std::bind(&TcpConnection::on_write, this, _1));
	channel_.set_close_callback(std::bind(&TcpConnection::on_close, this, _1));
}

TcpConnection::~TcpConnection() {
}

void TcpConnection::close() {
	socket_.close();
}

void TcpConnection::shutdown_read() {
	socket_.shutdownread();
}

void TcpConnection::shutdown_write() {
	socket_.shutdownwrite();
}

void TcpConnection::write(const char *buffer, size_t len) {
	if (write_buffer_.size()) {
	    assert(channel_.events() & EVENT_WRITE);
		// TODO limit size
	    write_buffer_.insert(write_buffer_.end(), buffer, buffer + len);
	} else {
	    assert((channel_.events() & EVENT_WRITE) == 0);
	    ssize_t nsend = socket_.send(buffer, len);
	    if (nsend < 0) {
			LOG(Error) << strerror(errno);
	    } else if (nsend < static_cast<int>(len)) {
			buffer += nsend;
			len -= nsend;
	    	write_buffer_.insert(write_buffer_.end(), buffer, buffer + len);
	    	channel_.enable_write();
	    } else {
	    	if (write_complete_cb_) {
	    		write_complete_cb_(this);
	    	}
	    }
	}
}

void TcpConnection::on_read(int fd) {
	assert(fd == socket_.fd());
	static_cast<void>(fd);

	// TODO use buffer
	char buffer[1024];
	ssize_t nread = socket_.recv(buffer, sizeof buffer);
	if (nread < 0) {
		perror("TcpConnection::recv");
		// TODO how to handle?
	} else if (nread == 0) {
		on_close(fd);
	} else {
		read_buffer_.insert(read_buffer_.end(), buffer, buffer + nread);
		if (message_cb_) {
			message_cb_(this);
		}
	}
}

void TcpConnection::on_write(int fd) {
	assert(fd == socket_.fd());
	static_cast<void>(fd);
	assert(write_buffer_.size());
	// TODO send data. if done, call write complete callback
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
		} else {
			// TODO more efficient
			auto begin = write_buffer_.begin();
			write_buffer_.erase(begin, begin + nsend);
		}
	}
}

void TcpConnection::on_close(int fd) {
	static_cast<void>(fd);
	channel_.disable_read();
	connected_ = false;
	if (close_cb_) {
		close_cb_(this);
	}
}

void TcpConnection::connection_established() {
	connected_ = true;
	if (connection_cb_) {
		connection_cb_(this); 
	}
}

} // namespace net
} // namespace sduzh
