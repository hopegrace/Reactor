#ifndef REACTOR_NET_HTTP_MIMETYPES_H
#define REACTOR_NET_HTTP_MIMETYPES_H

#include <string>
#include <map>
#include <vector>

namespace reactor {
namespace net {
namespace http {

void add_type(const std::string &type, const std::string &extension);
std::string guess_type(const std::string &path);
std::string guess_extension(const std::string &type);
std::vector<std::string> guess_all_extensions(const std::string &type);
std::map<std::string, std::string> read_mime_types(const std::string &path);

} // namespace http
} // namespace net
} // namespace reactor

#endif
