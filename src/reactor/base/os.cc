#include "reactor/base/os.h"

#include <string.h>
#include <sys/types.h>
#include <dirent.h>

namespace reactor {
namespace base {
namespace os {


void listdir(const std::string &path, PathList *result) {
	DIR *dir = ::opendir(path.c_str());
	if (dir == 0)
		return;
	struct dirent *entry = 0; 
	while ((entry=::readdir(dir)) != 0) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		result->push_back(entry->d_name);
	}
	closedir(dir);
}

PathList listdir(const std::string &path) {
	PathList result;
	listdir(path, &result);
	return result;
}

} // namespace os
} // namespace base
} // namespace reactor
