#include <ownlib/net/TcpConnection.h>

namespace sduzh {
namespace net {

TcpConnection::TcpConnection(EventLoop *loop, int fd):
		loop_(loop),
		socket_(fd),
		channel_(loop, fd),
		msg_cb_(),
		read_buffer_(),
		write_buffer_() {
	using namespace std::placeholders;
	channel_.set_read_callbac(std::bind(&TcpConnection::on_readable, this, _1));
	channel_.set_write_callback(std::bind(&TcpConnection::on_writable, this, _1));
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
	// TODO callback
	// if (write_buffer_.size()) {
	//     assert(channel_.events() & EVENT_WRITABLE);
	//     write_buffer_.append(buffer, len);
	// } else {
	//     assert((channel_.events() & EVENT_WRITABLE) == 0);
	//     ssize_t nsend = socket_.send(buffer, len);
	//     if (nsend < 0) {
	//         // error callback?
	//     } else if (nsend < len) {
	//     	   write_buffer_.append(buffer + nsend, len - nsend);
	//     	   channel_.enable_write();
	//     } else {
	//     	   if (write_complete_cb_) {
	//     	       write_complete_cb_(this);
	//     	   }
	//     }
	// }
}

void TcpConnection::on_readable(int fd) {
	assert(fd == socket_.fd());
	static_cast<void>(fd);

	// TODO read data, call message callback
}

void TcpConnection::on_writable(int fd) {
	assert(fd == socket_.fd());
	static_cast<void>(fd);
	assert(write_buffer_.size());
	// TODO send data. if done, call write complete callback
}

} // namespace net
} // namespace sduzh
