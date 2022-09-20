#include<iostream>
#include <string>
#include "util.hpp"
namespace ns_log {
    using namespace ns_util;
    enum {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };
    inline std::ostream& Log(const std::string& level, const std::string& file_name, int line) {
        std::string message = "[";
        message += level;
        message += "]";

        message += "[";
        message += file_name;
        message += "]";

        message += "[";
        message += std::to_string(line);
        message += "]";

        message += "[";
        message += TimeUtil::GetTimeStap();
        message += "]";

        std::cout << message;
        return std::cout;
    }
    //开放式日志
    //Log(INFO)<<message<<std::endl;
#define LOG(level) Log(#level,__FILE__,__LINE__)
}