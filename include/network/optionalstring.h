#ifndef NETWORK_OPTIONALSTRING_H
#define NETWORK_OPTIONALSTRING_H

#include "network/optional.h"   // Optional

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class OptionalString :
        public Optional
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const OptionalString& string);

    public:
        OptionalString() = default;
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::string& t_string);
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const char* t_value);
        OptionalString& operator=(const std::string& t_string);
        OptionalString& operator=(const char* t_value);
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
                                    const OptionalString& string);
}

#endif
