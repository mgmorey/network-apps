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
        OptionalString(const OptionalString&) = default;
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::nullopt_t& t_value);
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::string& t_value);
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const char* t_value);
        OptionalString& operator=(const OptionalString&) = default;
        OptionalString& operator=(const std::nullopt_t& t_value);
        OptionalString& operator=(const std::string& t_value);
        OptionalString& operator=(const char* t_value);
        operator std::string() const;
        operator const char*() const;
        bool has_value() const;
        std::string::size_type size() const;
        std::string value() const;
        std::string value_or(const std::string& t_value) const;

    private:
        std::optional<std::string> m_value {};
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const OptionalString& string);
}

#endif
