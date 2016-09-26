#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/Buffer.h>

namespace sduzh {
namespace net {

Buffer::Buffer(size_t size):
	data_(size > kInitialReadIndex ? size : kInitialReadIndex + 1024),
	rindex_(kInitialReadIndex),
	windex_(kInitialReadIndex) {

}

int Buffer::read_fd(int fd) {
	char buff[65536];
	size_t writables = writable_bytes();
	struct iovec buffers[2];
	buffers[0].iov_base = begin_write();
	buffers[0].iov_len = writables;
	buffers[1].iov_base = buff;
	buffers[1].iov_len = sizeof(buff);

	int nread = static_cast<int>(::readv(fd, buffers, 2));
	if (nread < 0) {
		LOG(Error) << "Buffer::read_fd" << strerror(errno);

	} else if (nread <= static_cast<int>(writables)) {
		has_written(nread);

	} else { 
		has_written(writables);
		append(buff, nread - writables);
	}
	return nread;
}

} // namespace net
} // namespace sduzh
