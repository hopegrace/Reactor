/*
 *
 * DO NOT use this (not finished)
 *
 */
#ifndef REACTOR_NET_HTTP_COOKIE_H
#define REACTOR_NET_HTTP_COOKIE_H

#include <string>

namespace reactor {
namespace net {
namespace http {

class Cookie {
public:
	Cookie(const std::string &name, const std::string &value):
		name_(name), value_(value) 
	{
	}

	~Cookie() = default;
	Cookie(const Cookie &) = default;
	Cookie &operator=(const Cookie &) = default;

	std::string to_string() const;

	/// getters
	const std::string &name() const
	{ return name_; }
	const std::string &value() const
	{ return value_; }
	const std::string &path() const
	{ return path_; }
	const std::string &domain() const
	{ return domain_; }
	time_t expires() const
	{ return expires_; }
	const std::string &raw_expires() const 
	{ return raw_expires_; }
	int max_age() const 
	{ return max_age_; }
	bool secure() const
	{ return secure_; }
	bool httponly() const
	{ return httponly_; }

	/// setters
	void set_name(const std::string &name)
	{ name_ = name; }
	void set_value(const std::string &value)
	{ value_ = value; }
	void set_path(const std::string &path)
	{ path_ = path; }
	void set_domain(const std::string &domain)
	{ domain_ = domain; }
	void set_expires(time_t expires)
	{ expires_ = expires; }
	// set_raw_expires..
	void set_max_age(int max_age)
	{ max_age_ = max_age; }
	void set_secure(bool secure)
	{ secure_ = secure; }
	void set_httponly(bool httponly)
	{ httponly_ = httponly; }

private:
	std::string name_;
	std::string value_;
	std::string path_; // optional
	std::string domain_; // optional
	time_t expires_; // optional
	std::string raw_expires_; // optional
	// MaxAge=0 means no 'Max-Age' attribute specified.
	// MaxAge<0 means delete cookie now, equivalently 'Max-Age: 0'
	// MaxAge>0 means Max-Age attribute present and given in seconds
	int max_age_;
	bool secure_;
	bool httponly_;
};


// read the 'Set-Cookie' value
bool read_set_cookie(const std::string &raw, Cookie *cookie);

} // namespace http
} // namespace net
} // namespace reactor

#endif
