#ifndef SDUZH_OWNLIB_BASE_SIMPLELOGGER_H
#define SDUZH_OWNLIB_BASE_SIMPLELOGGER_H

#include <functional>
#include <string>

typedef std::function<void (const char *, unsigned int)> OutputFunc;
typedef std::function<void ()> FlushFunc;

namespace reactor {
namespace base {

class SimpleLogger
{
public:
    typedef SimpleLogger Self;
    enum LogLevel
    {
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    SimpleLogger(LogLevel level, const char *file, const char *func, unsigned int line);
    ~SimpleLogger();

    static void setOutput(OutputFunc func);
    static void setFlush(FlushFunc flush);

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

	Self & operator << (int);
    Self & operator << (long);
    Self & operator << (double);
    Self & operator << (const char *);
    Self & operator << (const std::string &);

private:
    static LogLevel     g_logLevel;
    static FlushFunc    g_flushFunc;
    static OutputFunc   g_outputFunc;

    LogLevel    level_;
    const char  *file_;
    const char  *func_;
    unsigned int line_;

    std::string buff_;
};

} // namespace base
} // namespace reactor

#define Debug   reactor::base::SimpleLogger::Debug
#define Info    reactor::base::SimpleLogger::Info
#define Warn    reactor::base::SimpleLogger::Warn
#define Error   reactor::base::SimpleLogger::Error
#define Fatal   reactor::base::SimpleLogger::Fatal


#define LOG(level)  if (level >= reactor::base::SimpleLogger::logLevel()) \
	reactor::base::SimpleLogger(level, __FILE__, "", __LINE__)


#endif 
