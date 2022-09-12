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

#ifndef NETWORK_SIZEERROR_H
#define NETWORK_SIZEERROR_H

#include "network/rangeerror.h"         // RangeError, std::string

namespace Network
{
    class SizeError :
        public RangeError
    {
    public:
        static auto format(const std::string& t_value) -> std::string;
        explicit SizeError(const std::string& t_value) noexcept;
        explicit SizeError(std::string&& t_value) noexcept;
        SizeError(const SizeError&) noexcept = default;
        SizeError(SizeError&&) noexcept = default;
        ~SizeError() noexcept override = default;
        auto operator=(const SizeError&) noexcept -> SizeError& = default;
        auto operator=(SizeError&&) noexcept -> SizeError& = default;
    };
}

#endif
