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

#ifndef NETWORK_OSERRORRESULT_H
#define NETWORK_OSERRORRESULT_H

#include "network/os-error-type.h"      // os_error_type

#include <string>       // std::string
#include <utility>      // std::move()

namespace Network
{
    struct OsErrorResult
    {
        OsErrorResult() = default;

        OsErrorResult(os_error_type t_number,
                      const std::string& t_string);
        OsErrorResult(os_error_type t_number,
                      std::string&& t_string);
        auto operator==(const OsErrorResult& t_result) const noexcept -> bool;
        auto operator!=(const OsErrorResult& t_result) const noexcept -> bool;
        operator bool() const noexcept;  // NOLINT
        [[nodiscard]] auto number() const noexcept -> os_error_type;
        [[nodiscard]] auto string() const -> const std::string&;

    private:
        os_error_type m_number {0};
        std::string m_string {};
    };
}

#endif
