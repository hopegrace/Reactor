#include "DateTime.h"
#include <stdio.h>
#include <sys/time.h>


namespace sduzh {
namespace base {

const char *DateTime::kFormat = "%4d-%02d-%02d %02d:%02d:%02d.%06d";

DateTime::DateTime(time_t t, int ms):
    seconds_(t),
    micro_seconds_(ms)
{
}

DateTime::DateTime(int y, int M, int d, int h, int m, int s, int ms):
    seconds_(0),
    micro_seconds_(ms)
{
    struct tm tm_time;
    tm_time.tm_sec = s;
    tm_time.tm_min = m;
    tm_time.tm_hour = h;
    tm_time.tm_mday = d;
    tm_time.tm_mon = M - 1;
    tm_time.tm_year = y - 1900;
    tm_time.tm_isdst = 0;

    seconds_ = mktime(&tm_time);
}

const char *DateTime::to_string() const
{
    struct tm tm_time;
    localtime_r(&seconds_, &tm_time);
    snprintf(stime_, sizeof stime_, 
			 kFormat,
             tm_time.tm_year + 1900,
             tm_time.tm_mon + 1,
             tm_time.tm_mday,
             tm_time.tm_hour,
             tm_time.tm_min,
             tm_time.tm_sec,
             micro_seconds_);
    return stime_;
}

DateTime DateTime::current()
{
    struct timeval curr;
    ::gettimeofday(&curr, NULL);
    return DateTime(curr.tv_sec, static_cast<int>(curr.tv_usec));
}

DateTime DateTime::add_time(const DateTime &add)
{
   size_t ms = micro_seconds_ + add.micro_seconds_;
   time_t s = seconds_ + add.seconds_;
   return DateTime(s + ms/kMicroSecondsPerSecond, static_cast<int>(ms % kMicroSecondsPerSecond));
}

} // namespace base
} // namespace sduzh
