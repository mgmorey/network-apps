#ifndef NETWORK_STRING_H
#define NETWORK_STRING_H

#include "network-optional.h"   // Optional

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class String :
        public Optional
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const String& string);

    public:
        String();
        // cppcheck-suppress noExplicitConstructor
        String(const std::string& t_string);
        // cppcheck-suppress noExplicitConstructor
        String(const char* t_value);
        String& operator=(const std::string& t_string);
        String& operator=(const char* t_value);
        operator std::string() const;
        operator const char*() const;
        bool empty() const;

    private:
        std::string m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const String& string);
}

#endif
