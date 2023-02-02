// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SUNLENGTHERROR_H
#define NETWORK_SUNLENGTHERROR_H

#include "network/rangeerror.h"         // RangeError, std::string

namespace Network
{
#ifndef WIN32
    class SunLengthError :
        public RangeError
    {
    public:
        static auto format(const std::string& t_value) -> std::string;
        explicit SunLengthError(const std::string& t_value) noexcept;
        explicit SunLengthError(std::string&& t_value) noexcept;
        SunLengthError(const SunLengthError&) noexcept = default;
        SunLengthError(SunLengthError&&) noexcept = default;
        ~SunLengthError() noexcept override = default;
        auto operator=(const SunLengthError&) noexcept ->
            SunLengthError& = default;
        auto operator=(SunLengthError&&) noexcept ->
            SunLengthError& = default;
    };
#endif
}

#endif
