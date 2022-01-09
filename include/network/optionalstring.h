// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
        OptionalString(OptionalString&&) noexcept = default;
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::nullopt_t& t_value);		// NOLINT
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const std::string& t_value);		// NOLINT
        // cppcheck-suppress noExplicitConstructor
        OptionalString(const char* t_value);			// NOLINT
        ~OptionalString() = default;
        auto operator=(const OptionalString&) -> OptionalString& = default;
        auto operator=(OptionalString&&) noexcept -> OptionalString& = default;
        // NOLINTNEXTLINE
        auto operator=(const std::nullopt_t& t_value) -> OptionalString&;
        // NOLINTNEXTLINE
        auto operator=(const std::string& t_value) -> OptionalString&;
        // NOLINTNEXTLINE
        auto operator=(const char* t_value) -> OptionalString&;
        operator std::string() const;				// NOLINT
        operator const char*() const;				// NOLINT
        explicit operator bool() const;
        [[nodiscard]] auto has_value() const -> bool;
        [[nodiscard]] auto value() const -> std::string;
        [[nodiscard]] auto value_or(const std::string& t_value) const ->
            std::string;

    private:
        std::optional<std::string> m_value {};
    };

    extern auto operator==(const OptionalString& left,
                           const OptionalString& right) -> bool;
    extern auto operator!=(const OptionalString& left,
                           const OptionalString& right) -> bool;
    extern auto operator<<(std::ostream& os,
                           const OptionalString& string) -> std::ostream&;
}

#endif
