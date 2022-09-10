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

#ifndef NETWORK_RANGEERROR_H
#define NETWORK_RANGEERROR_H

#include "network/error.h"              // Error, std::string

namespace Network
{
    class RangeError :
        public Error
    {
    public:
        explicit RangeError(const std::string& t_str) noexcept;
        explicit RangeError(std::string&& t_str) noexcept;
        RangeError(const RangeError&) noexcept = default;
        RangeError(RangeError&&) noexcept = default;
        ~RangeError() noexcept override = default;
        auto operator=(const RangeError&) noexcept -> RangeError& = default;
        auto operator=(RangeError&&) noexcept -> RangeError& = default;
    };
}

#endif
