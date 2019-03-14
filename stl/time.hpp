#ifndef __STL_SLEEP_HPP
#define __STL_SLEEP_HPP

#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>

namespace stl
{
    namespace time
    {
        inline void
            sleep(unsigned long millseconds)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(millseconds));
        }

        template< typename T=unsigned long >
        inline T
        tick()
        {
            auto now = std::chrono::time_point_cast<std::chrono::duration<T, std::milli>>
                        (std::chrono::steady_clock::now());
            return now.time_since_epoch().count();
        }

        struct tmExtend: public std::tm
        {
            int tm_mill;
        };

        inline tmExtend 
            GetLocalTime(void)
        {
            auto clock = std::chrono::system_clock::now();
            auto epoch = clock.time_since_epoch();
            auto time = std::chrono::system_clock::to_time_t(clock);
            auto localtime = std::localtime(&time);
            tmExtend tmp;
            memcpy(&tmp, localtime, sizeof(std::tm));
            tmp.tm_year += 1900;
            tmp.tm_mon += 1;
            tmp.tm_mill = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count()%1000;
            return tmp;
        }

        inline std::string 
            GetLocalTime(const std::string& fmt)
        {
            std::unique_ptr<char[]> buffer(new char[1024]());
            tmExtend tm = GetLocalTime();
            sprintf(&buffer[0], fmt.c_str(), \
                    tm.tm_year, tm.tm_mon, tm.tm_mday, \
                    tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mill);
            return &buffer[0];
        }

		inline std::string 
			GetLocalTime(const std::string& fmt, const tmExtend& tm)
		{
			std::unique_ptr<char[]> buffer(new char[1024]());
			sprintf(&buffer[0], fmt.c_str(), \
				tm.tm_year, tm.tm_mon, tm.tm_mday, \
				tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mill);
			return &buffer[0];
		}
        
    } // time
}

#endif