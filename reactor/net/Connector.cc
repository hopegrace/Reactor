#include <reactor/net/Connector.h>

#include <assert.h>
#include <errno.h>
#include <reactor/net/EventLoop.h>

namespace sduzh {
namespace net {

Connector::Connector(EventLoop *loop, const InetAddress &servaddr):
	loop_(loop),
	servaddr_(servaddr),
	socket_(),
	channel_(),
	state_(State::Disconnected) {

}

Connector::~Connector() {
}

void Connector::connect() {
	if (state_ == State::Disconnected) {
		state_ = State::Connecting;
		do_connect();
	}
}

void Connector::do_connect() {
	assert(state_ == State::Connecting);
	socket_.reset(new TcpSocket());
	socket_->set_blocking(false);

	int ret = socket_->connect(servaddr_);
	int saved_errno = (ret == 0) ? 0 : errno;
	switch (saved_errno) {
	case 0:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		// TODO
	default:
		// TODO
	}
}

void Connector::on_write() {
	assert(state_ == State::Connecting);
	assert(socket_ && socket_->fd() >= 0);
	assert(channel_ && channel_->events());
}


} // namespace net
} // namespace sduzh
