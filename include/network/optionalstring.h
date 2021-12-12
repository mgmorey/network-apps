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
        auto operator=(const OptionalString&) -> OptionalString& = default;
        auto operator=(OptionalString&&) -> OptionalString& = default;
        auto operator=(const std::nullopt_t& t_value) ->
            OptionalString&;  // NOLINT
        auto operator=(const std::string& t_value) ->
            OptionalString&;  // NOLINT
        auto operator=(const char* t_value) ->
            OptionalString&;  // NOLINT
        operator std::string() const;  // NOLINT
        operator const char*() const;  // NOLINT
        [[nodiscard]] auto has_value() const -> bool;
        [[nodiscard]] auto size() const -> std::string::size_type;
        [[nodiscard]] auto value() const -> std::string;
        [[nodiscard]] auto value_or(const std::string& t_value) const ->
            std::string;

    private:
        std::optional<std::string> m_value {};
    };

    extern auto operator<<(std::ostream& os,
                           const OptionalString& string) -> std::ostream&;
}

#endif
