#ifndef NETWORK_STRING_H
#define NETWORK_STRING_H

#include "network-byte.h"       // Byte
#include "network-optional.h"   // Optional

#include <ostream>      // std::ostream
#include <string>       // std::basic_string, std::string

namespace Network
{
    using ByteString = std::basic_string<Byte>;

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
        char* data();
        const char* data() const;
        bool empty() const;
        std::string::size_type length() const;
        std::string::size_type size() const;

    private:
        std::string m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const String& string);
}

#endif
