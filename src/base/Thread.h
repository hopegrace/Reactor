#ifndef SDUZH_BASE_THREAD_H
#define SDUZH_BASE_THREAD_H

#include <string>
#include <functional>
#include <pthread.h>

namespace sduzh {
namespace base {

class Thread {
public:
	typedef std::string 			string_t;
	typedef std::function<void ()>  ThreadFunc_t;

	Thread(const string_t &name=string_t());
	Thread(const ThreadFunc_t &target, const string_t &name=string_t());
	~Thread();

	Thread(const Thread &) = delete;
	Thread &operator = (const Thread &) = delete;

	bool alive() const;
	bool daemon() const;
	int join();
	string_t name() const;
	
	void set_name(const string_t &name);
	void set_daemon(bool daemonic);

	void start();

	virtual void run();

private:
	static int thread_count;

	ThreadFunc_t target_;
	string_t     name_;
	pthread_t 	 tid_;	
};

} // namespace base
} // namespace sduzh

#endif
