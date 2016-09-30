#include "DateTime.h"
#include <stdio.h>
#include <sys/time.h>


namespace sduzh {

const char *DateTime::kFormat = "%4d-%02d-%02d %02d:%02d:%02d.%06d";

DateTime::DateTime():
	seconds_(0),
	micro_seconds_(0) {
	
}

DateTime::DateTime(time_t s, time_t us):
    seconds_(s),
    micro_seconds_(us) {
		
}

DateTime::DateTime(int y, int M, int d, int h, int m, int s, int us):
    seconds_(0),
    micro_seconds_(us)
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

DateTime DateTime::add_time(const DateTime &add) const
{
	time_t x = add.micro_seconds() / kMicroSecondsPerSecond;
	time_t y = add.micro_seconds() % kMicroSecondsPerSecond;
	time_t s = seconds_ + x;
	time_t us = micro_seconds_ + y;
	if (us > kMicroSecondsPerSecond) {
		s += 1;
		us -= kMicroSecondsPerSecond;
	}
	return DateTime(s, us);
}

DateTime DateTime::add_seconds(double add) const
{
	//FIXME when add < 0
	//may lose precision
	time_t delta = static_cast<time_t>(add * kMicroSecondsPerSecond);
	return add_time(DateTime(delta / kMicroSecondsPerSecond, delta % kMicroSecondsPerSecond));
}

} // namespace sduzh
