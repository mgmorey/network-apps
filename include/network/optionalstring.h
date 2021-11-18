#ifndef NETWORK_OPTIONALSTRING_H
#define NETWORK_OPTIONALSTRING_H

#include <optional>     // std::nullopt_t, std::optional
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class OptionalString
    {
    public:
        OptionalString() = default;
        OptionalString(const OptionalString&) = default;
        OptionalString(OptionalString&&) = default;
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::nullopt_t& t_value);  // NOLINT
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::string& t_value);  // NOLINT
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const char* t_value);  // NOLINT
        ~OptionalString() = default;
        OptionalString& operator=(const OptionalString&) = default;
        OptionalString& operator=(OptionalString&&) = default;
        OptionalString& operator=(const std::nullopt_t& t_value);  // NOLINT
        OptionalString& operator=(const std::string& t_value);  // NOLINT
        OptionalString& operator=(const char* t_value);  // NOLINT
        operator std::string() const;  // NOLINT
        operator const char*() const;  // NOLINT
        [[nodiscard]] bool has_value() const;
        [[nodiscard]] std::string::size_type size() const;
        [[nodiscard]] std::string value() const;
        [[nodiscard]] std::string value_or(const std::string& t_value) const;

    private:
        std::optional<std::string> m_value {};
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const OptionalString& string);
}

#endif
