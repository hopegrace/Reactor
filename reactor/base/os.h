#ifndef REACTOR_BASE_OS_H
#define REACTOR_BASE_OS_H

#include <vector>
#include <string>

namespace reactor {
namespace base {
namespace os {

typedef std::vector<std::string> PathList;

PathList listdir(const std::string &path);
void listdir(const std::string &path, PathList *result);

} // namespace os
} // namespace base
} // namespace reactor

#endif
