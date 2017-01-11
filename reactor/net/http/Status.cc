#include "Status.h"
#include <unordered_map>

namespace reactor {
namespace net {
namespace http {

using namespace std;

static unordered_map<int, string> g_status_text;

class MapInitializer {
public:
	MapInitializer() {
		g_status_text[StatusOk]       = "OK";
		g_status_text[StatusNotFound] = "NotFound";
	}
}init;

string status_text(int status_code)
{
	auto it = g_status_text.find(status_code);
	return (it == g_status_text.end()) ? "" : it->second;
}

} // namespace http
} // namespace net
} // namespace reactor
