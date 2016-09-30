#ifndef SDUZH_BASE_DATETIME_H
#define SDUZH_BASE_DATETIME_H

#include <string>
#include <time.h>

namespace sduzh {

///
/// FIXME bad API
/// 
class DateTime
{
public:
    static const int kMicroSecondsPerSecond = 1000 * 1000; // 10^6

	DateTime();
    DateTime(time_t seconds, time_t us);
    DateTime(int y, int M, int d, int h, int m, int s, int us=0);

    inline time_t micro_seconds() const { return seconds_ * kMicroSecondsPerSecond + micro_seconds_; }
    const char *to_string() const;

    static DateTime current();

    DateTime add_time(const DateTime &add) const;
	DateTime add_seconds(double add) const;

	DateTime operator + (const DateTime &rhs) const { 
		return add_time(rhs);
	}

	void operator += (const DateTime &rhs) {
		*this = *this + rhs;
	}

private:
    static const char *kFormat;

    time_t seconds_;
    size_t micro_seconds_; // micro seconds(us)

	mutable char stime_[32];
};

inline bool operator == (const DateTime &lhs, const DateTime &rhs) {
	return lhs.micro_seconds() == rhs.micro_seconds();
}

inline bool operator != (const DateTime &lhs, const DateTime &rhs) {
	return !(lhs == rhs);
}

inline bool operator < (const DateTime &lhs, const DateTime &rhs) {
	return lhs.micro_seconds() < rhs.micro_seconds();
}

inline bool operator <= (const DateTime &lhs, const DateTime &rhs) {
	return (lhs < rhs) || (lhs == rhs);
}

inline bool operator > (const DateTime &lhs, const DateTime &rhs) {
	return !(lhs <= rhs);
}

inline bool operator >= (const DateTime &lhs, const DateTime &rhs) {
	return (lhs > rhs) || (lhs == rhs);
}

} // namespace sduzh

#endif // 
