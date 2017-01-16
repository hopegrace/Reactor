#include "Status.h"
#include <unordered_map>

namespace reactor {
namespace net {
namespace http {

static std::unordered_map<int, const char *> g_status_text = {
	{StatusContinue,           "Continue"},
    {StatusSwitchingProtocols, "Switching Protocols"},
    {StatusProcessing,         "Processing"},
    
    {StatusOK,                   "OK"},
    {StatusCreated,              "Created"},
    {StatusAccepted,             "Accepted"},
    {StatusNonAuthoritativeInfo, "Non-Authoritative Information"},
    {StatusNoContent,            "No Content"},
    {StatusResetContent,         "Reset Content"},
    {StatusPartialContent,       "Partial Content"},
    {StatusMultiStatus,          "Multi-{Status"},
    {StatusAlreadyReported,      "Already Reported"},
    {StatusIMUsed,               "IM Used"},
    
    {StatusMultipleChoices,   "Multiple Choices"},
    {StatusMovedPermanently,  "Moved Permanently"},
    {StatusFound,             "Found"},
    {StatusSeeOther,          "See Other"},
    {StatusNotModified,       "Not Modified"},
    {StatusUseProxy,          "Use Proxy"},
    {StatusTemporaryRedirect, "Temporary Redirect"},
    {StatusPermanentRedirect, "Permanent Redirect"},
   
	{StatusBadRequest,                   "Bad Request"},
    {StatusUnauthorized,                 "Unauthorized"},
    {StatusPaymentRequired,              "Payment Required"},
    {StatusForbidden,                    "Forbidden"},
    {StatusNotFound,                     "Not Found"},
    {StatusMethodNotAllowed,             "Method Not Allowed"},
    {StatusNotAcceptable,                "Not Acceptable"},
    {StatusProxyAuthRequired,            "Proxy Authentication Required"},
    {StatusRequestTimeout,               "Request Timeout"},
    {StatusConflict,                     "Conflict"},
    {StatusGone,                         "Gone"},
    {StatusLengthRequired,               "Length Required"},
    {StatusPreconditionFailed,           "Precondition Failed"},
    {StatusRequestEntityTooLarge,        "Request Entity Too Large"},
    {StatusRequestURITooLong,            "Request URI Too Long"},
    {StatusUnsupportedMediaType,         "Unsupported Media Type"},
    {StatusRequestedRangeNotSatisfiable, "Requested Range Not Satisfiable"},
    {StatusExpectationFailed,            "Expectation Failed"},
    {StatusTeapot,                       "I'm a teapot"},
    {StatusUnprocessableEntity,          "Unprocessable Entity"},
    {StatusLocked,                       "Locked"},
    {StatusFailedDependency,             "Failed Dependency"},
    {StatusUpgradeRequired,              "Upgrade Required"},
    {StatusPreconditionRequired,         "Precondition Required"},
    {StatusTooManyRequests,              "Too Many Requests"},
    {StatusRequestHeaderFieldsTooLarge,  "Request Header Fields Too Large"},
    {StatusUnavailableForLegalReasons,   "Unavailable For Legal Reasons"},
    
    {StatusInternalServerError,           "Internal Server Error"},
    {StatusNotImplemented,                "Not Implemented"},
    {StatusBadGateway,                    "Bad Gateway"},
    {StatusServiceUnavailable,            "Service Unavailable"},
    {StatusGatewayTimeout,                "Gateway Timeout"},
    {StatusHTTPVersionNotSupported,       "HTTP Version Not Supported"},
    {StatusVariantAlsoNegotiates,         "Variant Also Negotiates"},
    {StatusInsufficientStorage,           "Insufficient Storage"},
    {StatusLoopDetected,                  "Loop Detected"},
    {StatusNotExtended,                   "Not Extended"},
    {StatusNetworkAuthenticationRequired, "Network Authentication Required"},
};

const char *status_text(int status_code)
{
	auto it = g_status_text.find(status_code);
	return (it == g_status_text.end()) ? "" : it->second;
}

} // namespace http
} // namespace net
} // namespace reactor
