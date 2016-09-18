#ifndef SDUZH_OWNLIB_NET_BUFFER_H
#define SDUZH_OWNLIB_NET_BUFFER_H

#include <vector>

namespace sduzh {
namespace net {

class Buffer {
public:
	Buffer(size_t size);

	int read_fd(int fd);

	char *data() { return &data_[rindex_]; }
	void retrieve(size_t size);

	int readable_bytes() const { return windex_ - rindex_; }
	int writable_bytes() const { return static_cast<int>(data_.size()) - windex_; }
	bool empty() const { return readable_bytes() == 0; }

	void append(const char *data, size_t len);
	void clear();

private:
	const static int kInitialReadIndex = 8;
	const static int kMaxReadIndex = 1024 * 1024;
	
	std::vector<char> data_;
	int rindex_;
	int windex_; 
};

} // namespace net
} // namespace sduzh

#endif
