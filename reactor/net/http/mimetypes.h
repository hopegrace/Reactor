#ifndef REACTOR_NET_HTTP_MIMETYPES_H
#define REACTOR_NET_HTTP_MIMETYPES_H

#include <string>
#include <map>
#include <vector>

namespace reactor {
namespace net {
namespace http {

namespace mimetypes {

void add_type(const std::string &type, const std::string &extension);
void guess_type(const std::string &path, std::string *type, std::string *encoding);
void guess_extension(const std::string &type, std::string *extension);
void guess_all_extensions(const std::string &type, std::vector<std::string> *extensions);
void read_mime_types(const std::string &path, std::map<std::string, std::string> *mimetypes);

}

} // namespace http
} // namespace net
} // namespace reactor

#endif
