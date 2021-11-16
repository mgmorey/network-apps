#ifndef NETWORK_OPTIONALSTRING_H
#define NETWORK_OPTIONALSTRING_H

#include <optional>     // std::optional
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class OptionalString
    {
    public:
        OptionalString() = default;
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::string& t_string);
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const char* t_value);
        OptionalString& operator=(const std::string& t_string);
        OptionalString& operator=(const char* t_value);
        operator const char*() const;
        const char* data() const;
        bool empty() const;
        bool has_value() const;
        std::string::size_type length() const;
        std::string::size_type size() const;
        std::string value_or(const std::string& value) const;

    private:
        std::optional<std::string> m_value {};
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const OptionalString& string);
}

#endif
