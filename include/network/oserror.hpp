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

#ifndef NETWORK_OSERROR_HPP
#define NETWORK_OSERROR_HPP

#include "network/os-error-type.hpp"    // os_error_type

#include <string>       // std::string
#include <string_view>  // std::string_view

namespace Network
{
    struct OsError
    {
        OsError() = default;

        OsError(os_error_type t_number, std::string_view t_string);

        OsError(const OsError&) = default;
        OsError(OsError&&) = default;
        ~OsError() = default;
        auto operator=(const OsError&) -> OsError& = default;
        auto operator=(OsError&&) -> OsError& = default;

        operator bool() const noexcept;  // NOLINT
        [[nodiscard]] auto number() const noexcept -> os_error_type;
        [[nodiscard]] auto string() const -> const std::string&;

    private:
        os_error_type m_number {0};
        std::string m_string;
    };
}

#endif
