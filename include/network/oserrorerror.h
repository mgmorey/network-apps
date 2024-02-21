// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_OSERRORERROR_H
#define NETWORK_OSERRORERROR_H

#include "network/rangeerror.h"         // RangeError

#include <string>       // std::string

namespace Network
{
    class OsErrorError :
        public RangeError
    {
    public:
        explicit OsErrorError(const std::string& t_value) noexcept;
        OsErrorError(const OsErrorError&) noexcept = default;
        OsErrorError(OsErrorError&&) noexcept = default;
        ~OsErrorError() noexcept override = default;
        auto operator=(const OsErrorError&) noexcept ->
            OsErrorError& = default;
        auto operator=(OsErrorError&&) noexcept ->
            OsErrorError& = default;
    };
}

#endif
