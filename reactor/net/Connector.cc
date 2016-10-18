#include <reactor/net/Connector.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/EventLoop.h>
#include <reactor/net/SocketOpt.h>

namespace reactor {
namespace net {

Connector::Connector(EventLoop *loop, const InetAddress &servaddr):
	loop_(loop),
	servaddr_(servaddr),
	socket_(-1),
	channel_(),
	state_(State::Disconnected),
	delay_ms_(5) {

}

Connector::~Connector() {
	delete_channel();
}

void Connector::close_socket() {
	if (socket_ >= 0) {
		::close(socket_);
		socket_ = -1;
	}
}

void Connector::delete_channel() {
	if (channel_) {
		channel_->disable_all();
		channel_->remove();
		loop_->queue_run(std::bind(&Connector::_delete_channel, this));
	}
}

void Connector::_delete_channel() {
	if (channel_) {
		LOG(Debug) << "delete channel of fd " << channel_->fd();
		delete channel_;
		channel_ = 0;
	}
}

void Connector::restart() {
	// TODO
}

void Connector::start() {
	if (state_ == State::Disconnected) {
		state_ = State::Connecting;
		connect();
	} else {
		LOG(Debug) << "start() ignore, current state is " << str_state();
	}
}

void Connector::stop() {
	if (state_ == State::Connecting) {
		assert(channel_);
		assert(socket_ >= 0);
		delete_channel();
		close_socket();
		state_ = State::Disconnected;
	} else {
		LOG(Debug) << "cat not stop(), current state is " << str_state();
	}
}

void Connector::connect() {
	assert(state_ == State::Connecting);
	assert(socket_ < 0);
	assert(!channel_);
	socket_ = sockets::create_nonblock_socket();

	LOG(Info) << "connecting to " << servaddr_.to_string() << ", fd=" << socket_;
	int ret = sockets::connect(socket_, servaddr_);
	int saved_errno = (ret == 0) ? 0 : errno;
	switch (saved_errno) {
	case 0:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		connecting();
		break;

	case EAGAIN:
	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:
		LOG(Error) << strerror(saved_errno);
		retry();
		break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      LOG(Error) << strerror(saved_errno);
	  close_socket();
      break;

    default:
      LOG(Error) << "Unexpected error in Connector::connect() " << strerror(saved_errno);
	  close_socket();
      break;
	}
}

void Connector::connecting() {
	assert(!channel_);
	assert(socket_ >= 0);
	channel_ = new Channel(loop_, socket_);
	channel_->set_write_callback(std::bind(&Connector::on_write, this));
	channel_->set_error_callback(std::bind(&Connector::on_error, this));
	channel_->enable_write();
}

void Connector::on_error() {
	LOG(Error) << "socket error: " << sockets::get_socket_error(socket_);
	retry();
}

void Connector::on_write() {
	assert(state_ == State::Connecting);

	int err = sockets::get_socket_error(socket_);
	if (err) {
		LOG(Error) << "socket error " << err;
		retry();
	} else {
		state_ = State::Connected;
		delete_channel();
		LOG(Info) << "connect to " << servaddr_.to_string() << " succeeded";
		if (connection_cb_) {
			connection_cb_(socket_);
		} else {
			close_socket();
		}
	}
}

void Connector::retry() {
	assert(state_ == State::Connecting);
	close_socket();
	delete_channel();
	
	LOG(Info) << "retry connecting to " << servaddr_.to_string() << " in " << delay_ms_ << " ms";
	loop_->run_after(delay_ms_ / 1000.0, std::bind(&Connector::connect, this));
	delay_ms_ = 2 * delay_ms_; 
	if (delay_ms_ > 30 * 1000) { delay_ms_ = 30 * 1000; }
}

const char *Connector::str_state() {
	switch (state_) {
	case State::Disconnected: return "Disconnected";
	case State::Connecting:   return "Connecting";
	case State::Connected:    return "Connected";
	default:
		return "Unknown";
	}
}

} // namespace net
} // namespace reactor
