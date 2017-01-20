#include "path.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <vector>

#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "reactor/base/strings/strings.h"

namespace reactor {
namespace base {
namespace path {

using namespace std;

namespace detail {
std::string getcwd() 
{
	char buff[1024];
	if (::getcwd(buff, sizeof buff) == NULL) {
		::perror("getcwd");
		::abort();
	}
	return std::string(buff);
}
}

std::string abspath(const std::string &path)
{
	if (!isabs(path)) {
		return normpath(join(detail::getcwd(), path));
	}
	return normpath(path);
}

std::string basename(const std::string &path) 
{
	size_t i = path.rfind('/');
	return (i == string::npos) ? path : path.substr(i+1);
}

// commonprefix
std::string dirname(const std::string &path)
{
	size_t i = path.rfind('/');
	if (i == string::npos)
		return "";
	string dir = path.substr(0, i+1);
	if (count(dir.begin(), dir.end(), '/') != static_cast<int>(dir.size())) 
		dir.resize(dir.size() - 1);
	return dir;
}

bool exists(const std::string &path)
{
	return ::access(path.c_str(), F_OK) != -1;
}

std::string expanduser(const std::string &path)
{
	if (path.empty() || path[0] != '~')
		return path;

	size_t i = path.find('/', 1);
	if (i == string::npos) {
		i = path.size();
	}

	char *userhome = 0;
	if (i == 1) {
		userhome = ::getenv("HOME");	
		if (userhome == 0) {
			userhome = ::getpwuid(getuid())->pw_dir;
		}
	} else {
		struct passwd *passwd = ::getpwnam(path.substr(1, i).c_str());
		if (passwd == 0)
			return path;
		userhome = passwd->pw_dir;
	}

	string ret(strings::rstrip(userhome, '/') + path.substr(i));
	return ret.empty() ? "/" : ret;
}

std::string expandvars(const std::string &path)
{
	fprintf(stderr, "expandvars() not implement yet\n");
	abort();
	return "";
}

time_t getatime(const std::string &path)
{
	struct stat stat;
	::stat(path.c_str(), &stat);
	return stat.st_atime;
}

time_t getctime(const std::string &path)
{
	struct stat stat;
	::stat(path.c_str(), &stat);
	return stat.st_ctime;
}

time_t getmtime(const std::string &path)
{
	struct stat stat;
	::stat(path.c_str(), &stat);
	return stat.st_mtime;
}

size_t getsize(const std::string &path)
{
	struct stat stat;
	::stat(path.c_str(), &stat);
	return stat.st_size;
}

bool isabs(const std::string &path)
{
	return !path.empty() && path[0] == '/';
}

bool isdir(const std::string &path) 
{
	struct stat stat;
	return (::stat(path.c_str(), &stat) == 0) && S_ISDIR(stat.st_mode);
}

bool isfile(const std::string &path)
{
	struct stat stat;
	return (::stat(path.c_str(), &stat) == 0) && S_ISREG(stat.st_mode);
}

bool islink(const std::string &path)
{
	struct stat stat;
	return (::lstat(path.c_str(), &stat) == 0) && S_ISLNK(stat.st_mode);
}

bool ismount(const std::string &path)
{
	if (islink(path))
		return false;
	struct stat s1;
	struct stat s2;
	if (::lstat(path.c_str(), &s1) || ::lstat(join(path, "..").c_str(), &s2))
		return false;
	if (s1.st_dev != s2.st_dev)
		return true;
	if (s1.st_ino == s2.st_ino)
		return true;
	return false;
}

std::string join(const std::string &a, const std::string &b)
{
	if (!b.empty() && b[0] == '/')
		return b;
	if (a.empty() || a.back() == '/')
		return a + b;
	return a + "/" + b;
}

bool lexists(const std::string &path) 
{
	struct stat stat;
	return ::lstat(path.c_str(), &stat) == 0;
}

std::string normpath(const std::string &path) 
{
	if (path.empty())
		return ".";

	const char *p = path.c_str();
	int initial_slashes = (p[0] == '/') ? 1 : 0;
	if (initial_slashes && strstr(p, "//") == 0 && strstr(p, "///") != 0) {
		initial_slashes = 2;
	}
	vector<string> comps = strings::split(path, '/');
	vector<string> new_comps;
	for (const string &comp : comps) {
		if (comp.empty() || comp==".")
			continue;
		if ((comp != "..") 
				|| (!initial_slashes && new_comps.empty()) 
				|| (new_comps.size() && new_comps.back() == "..")) {
			new_comps.push_back(comp);
		} else if (new_comps.size()) {
			new_comps.pop_back();
		}
	}

	stringstream ss;
	for (int i = 0;i < initial_slashes; i++) {
		ss << '/';
	}

	for (int i = 0, sz = static_cast<int>(new_comps.size()); i < sz; i++) {
		ss << new_comps[i];
		if (i < sz-1) {
			ss << '/';
		}
	}

	string ret(ss.str());
	return ret.empty() ? "." : ret;
}

std::pair<std::string, std::string> split(const std::string &path) 
{
	size_t slash = path.rfind('/');
	if (slash == string::npos) 
		return make_pair("", path);

	string head = path.substr(0, slash+1);
	string tail = path.substr(slash+1);
	strings::rstrip(&head, '/');
	return head.empty() ? make_pair("/", tail) : make_pair(head, tail);
}

std::pair<std::string, std::string> splitext(const std::string &path)
{
	std::string basename = path::basename(path);
	size_t dot = basename.rfind('.');
	if (dot != string::npos) {
		// skip all leading dots
		for (size_t i = 0; i < dot; i++) {
			if (basename[i] != '.') 
				return make_pair(basename.substr(0, dot), basename.substr(dot));
		}
	}
	return make_pair(path, "");
}

// walk

} // namespace path
} // namespace base
} // namespace reactor
