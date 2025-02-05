#ifndef _LOG_
#define _LOG_

#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>

enum LogLevel {logERROR, logWARNING, logINFO, logDEBUG};

inline std::string GetTime();

class Log
{
    public:
        Log(LogLevel _loglevel);
        virtual ~Log();
    public:
        static LogLevel& ReportingLevel();
        static std::string ToString(LogLevel _loglevel);
        static LogLevel FromString(const std::string& _loglevel);
        template <typename T>
            Log& operator <<(T const& value)
            {
                _os << value;
                return *this;
            }
    protected:
        std::ostringstream _os;
    private:

};

inline Log::Log(LogLevel _loglevel)
{
    std::string lineColour = "";
    if (_loglevel == logDEBUG) lineColour = "\033[34m";
    else if (_loglevel == logINFO) lineColour = "\033[32m";
    else if (_loglevel == logWARNING) lineColour = "\033[33m";
    else if (_loglevel == logERROR) lineColour = "\033[31m";
    
    if (_loglevel != logDEBUG) _os << lineColour;
    // << "- " << GetTime();
    else {_os << lineColour;}
    _os << ToString(_loglevel) << ": ";
    _os << std::string(_loglevel > logDEBUG ? (_loglevel - logDEBUG) : 0, '\t');
}

inline Log::~Log()
{
    _os << "\033[0m" << std::endl;
    std::cout << _os.str();
}

inline LogLevel& Log::ReportingLevel()
{
    static LogLevel reportingLevel = logDEBUG;
    return reportingLevel;
}

inline std::string Log::ToString(LogLevel _loglevel)
{
    static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    return buffer[_loglevel];
}

inline LogLevel Log::FromString(const std::string& _loglevel)
{
    if (_loglevel == "DEBUG") return logDEBUG;
    if (_loglevel == "INFO") return logINFO;
    if (_loglevel == "WARNING") return logWARNING;
    if (_loglevel == "ERROR") return logERROR;
    Log(logWARNING) << "Unknown logging level '" << _loglevel << "'. Using INFO level as default.";
    return logINFO;
}

typedef Log LOG;

#define LOG(_loglevel) \
    if (_loglevel > LOG::ReportingLevel()) ;          \
else Log(_loglevel)

inline std::string GetTime()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    char buffer[100];
    tm r;
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&tv.tv_sec, &r));
    char result[100];
    sprintf(result, "%s", buffer);
    return result;
}

#endif /* _LOG_ */
