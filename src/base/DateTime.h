#ifndef SDUZH_BASE_DATETIME_H
#define SDUZH_BASE_DATETIME_H

#include <string>
#include <time.h>

namespace sduzh {
namespace base {

class DateTime
{
public:
    explicit DateTime(time_t seconds = 0, int ms=0);
    DateTime(int y, int M, int d, int h, int m, int s, int ms=0);

    inline time_t micro_seconds() const { return seconds_ * kMicroSecondsPerSecond + micro_seconds_; }
    std::string to_string() const;

    static DateTime current();

    inline bool operator < (const DateTime &other) {
        return micro_seconds() < other.micro_seconds();
    }

    inline bool operator == (const DateTime &other) {
        return seconds_ == other.seconds_ && micro_seconds_ == other.micro_seconds_;
    }

    inline bool operator != (const DateTime &other) {
        return !(*this==other);
    }

    DateTime add_time(const DateTime &add);

private:
    static const char *kFormat;
    static const int kMicroSecondsPerSecond = 1000 * 1000;

    time_t seconds_;
    size_t micro_seconds_; // micro seconds
};

} // namespace base
} // namespace sduzh

#endif // 
