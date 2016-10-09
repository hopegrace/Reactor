#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <reactor/net/Acceptor.h>
#include <reactor/base/SimpleLogger.h>

namespace reactor {
namespace net {

static void default_conn_callback(int fd, const InetAddress &addr, Timestamp time) {
	LOG(Info) << "[" << addr.to_string() << "] connected, fd=" << fd << " connect time " 
		<< time.to_string();
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &addr):
	loop_(loop),
	socket_(),
	channel_(loop, socket_.fd()),
	idle_fd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)),
	started_(false),
	new_connection_cb_(default_conn_callback) {

	assert(idle_fd_ >= 0);
	socket_.set_blocking(false);
	socket_.set_reuse_addr(true);
	socket_.bind(addr);

	channel_.set_read_callback(std::bind(&Acceptor::on_read, this));
}

void Acceptor::listen() {
	if (!started_) {
		socket_.listen(5);
		channel_.enable_read();
		started_ = true;
	}
}

void Acceptor::on_read() {
	Timestamp now(Timestamp::current());
	InetAddress addr;
	int fd = socket_.accept(&addr);
	if (fd >= 0) {
		new_connection_cb_(fd, addr, now);
	} else {
		LOG(Error) << "in Acceptor::on_read() " << strerror(errno);
		if (errno == EMFILE) {
			::close(idle_fd_);
			idle_fd_ = socket_.accept(&addr);
			::close(idle_fd_);
			idle_fd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
		}
	}
}

} // namespace net
} // namespace reactor
