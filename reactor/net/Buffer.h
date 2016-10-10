#ifndef SDUZH_OWNLIB_NET_BUFFER_H
#define SDUZH_OWNLIB_NET_BUFFER_H

#include <assert.h>
#include <algorithm>
#include <string.h>
#include <vector>

namespace reactor {
namespace net {

class Buffer {
public:
	Buffer(size_t size);

	int read_fd(int fd);

	char *data() { return begin_read(); }
	const char *data() const { return begin_read(); }

	size_t prependable_bytes() const { return rindex_; }
	size_t readable_bytes() const { return windex_ - rindex_; }
	size_t writable_bytes() const { return static_cast<int>(data_.size()) - windex_; }

	void retrieve(size_t size) {
		if (readable_bytes() <= size) {
			retrieve_all();
		} else {
			rindex_ += static_cast<int>(size);
		}
	}

	void retrieve_all() {
		rindex_ = kInitialReadIndex;
		windex_ = kInitialReadIndex;
	}

	bool empty() const { return readable_bytes() == 0; }

	void append(const std::string &s) {
		append(s.c_str(), s.size());
	}

	void append(const char *data, size_t len) {
		append(data, data + len);
	}

	void append(const void *data, size_t len) {
		append(static_cast<const char *>(data), len);
	}

	void append(const char *begin, const char *end) {
		size_t len = end - begin;
		ensure_enough_space(len);
		std::copy(begin, end, begin_write());
		has_written(len);
	}

	void prepend(const std::string &s) {
		prepend(s.c_str(), s.size());
	}
	
	void prepend(const char *s) {
		prepend(s, ::strlen(s));
	}

	void prepend(const void *data, size_t len) {
		prepend(static_cast<const char*>(data), len);
	}

	void prepend(const char *data, size_t len) {
		if (len <= prependable_bytes()) {
			rindex_ -= len;
			std::copy(data, data + len, begin_read());
		} else {
			size_t n = len - prependable_bytes();
			ensure_enough_space(n);
			std::move_backward(begin_read(), begin_write(), begin_read() + n);
			std::copy(data, data + len, begin());
			windex_ += n;
			rindex_ = 0;
		}
	}

	void clear() {
		retrieve_all();
	}

private:
	size_t all_writable_bytes() const {
		return (rindex_ - kInitialReadIndex) + writable_bytes();
	}

	char *begin() {
		return &*data_.begin();
	}

	const char *begin() const {
		return &*data_.begin();
	}

	char *begin_read() {
		return begin() + rindex_;
	}

	const char *begin_read() const { 
		return begin() + rindex_;
	}

	char *begin_write() {
		return begin() + windex_;
	}

	const char *begin_write() const {
		return begin() + windex_;
	}

	void has_written(size_t len) {
		assert(len <= writable_bytes());
		windex_ += len;
	}

	void unwrite(size_t len) {
		assert(len <= readable_bytes());
		windex_ -= len;
	}

	void ensure_enough_space(size_t len) {
		if (writable_bytes() < len) {
			make_space(len);
		}
	}

	void make_space(size_t len) {
		if (prependable_bytes() + writable_bytes() >= len + kInitialReadIndex) {
			size_t readable = readable_bytes();
			std::copy(data_.begin() + rindex_, 
					  data_.begin() + windex_,
					  data_.begin() + kInitialReadIndex);
			rindex_ = kInitialReadIndex;
			windex_ = kInitialReadIndex + readable;
		} else {
			data_.resize(windex_ + len);
		}
	}

	const static size_t kInitialReadIndex = 32;
	const static size_t kMaxReadIndex = 1024 * 1024;
	
	std::vector<char> data_;
	size_t rindex_;
	size_t windex_; 
};

} // namespace net
} // namespace reactor

#endif
