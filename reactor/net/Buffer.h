#ifndef SDUZH_OWNLIB_NET_BUFFER_H
#define SDUZH_OWNLIB_NET_BUFFER_H

#include <string.h>
#include <vector>

namespace sduzh {
namespace net {

class Buffer {
public:
	Buffer(size_t size);

	int read_fd(int fd);

	char *data() { 
		return &data_[rindex_]; 
	}

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

	void append(const char *data, size_t len) {
		append(data, data + len);
	}

	void append(const void *data, size_t len) {
		append(static_cast<const char *>(data), len);
	}

	void append(const char *begin, const char *end) {
		size_t len = end - begin;
		if (writable_bytes() < len && all_writable_bytes() >= len) {
			size_t wb = writable_bytes();
			::memmove(&data_[kInitialReadIndex], data(), wb);
			rindex_ = kInitialReadIndex;
			windex_ = kInitialReadIndex + wb;
		}
		data_.insert(data_.begin() + windex_, begin, end);
		windex_ += len;
	}

	void clear() {
		retrieve_all();
	}

private:
	size_t all_writable_bytes() const {
		return (rindex_ - kInitialReadIndex) + writable_bytes();
	}

	const static size_t kInitialReadIndex = 8;
	const static size_t kMaxReadIndex = 1024 * 1024;
	
	std::vector<char> data_;
	size_t rindex_;
	size_t windex_; 
};

} // namespace net
} // namespace sduzh

#endif
