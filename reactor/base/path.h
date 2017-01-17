#ifndef REACTOR_BASE_PATH_H
#define REACTOR_BASE_PATH_H

#include <string>

namespace reactor {
namespace base {
/*
 * see python os.path for API doc
 * XXX noly suppory english path name
 */
namespace path {

std::string abspath(const std::string &path);
std::string basename(const std::string &path);
// commonprefix
std::string dirname(const std::string &path);
bool exists(const std::string &path);
std::string expanduser(const std::string &path);
std::string expandvars(const std::string &path);
time_t getatime(const std::string &path);
time_t getctime(const std::string &path);
time_t getmtime(const std::string &path);
size_t getsize(const std::string &path);
bool isabs(const std::string &path);
bool isdir(const std::string &path);
bool isfile(const std::string &path);
bool islink(const std::string &path);
bool ismount(const std::string &path);
std::string join(const std::string &a, const std::string &b);
bool lexists(const std::string &path);
std::string normcase(const std::string &path);
std::string realpath(const std::string &path);
std::string relpath(const std::string &path, const std::string &start=".");
bool samefile(const std::string &a, const std::string &b);
// split
// splitdrive
// splitext
// walk

} // namespace path
} // namespace base
} // namespace reactor

#endif
