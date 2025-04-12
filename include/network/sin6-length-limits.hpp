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

#ifndef NETWORK_SIN6_LENGTH_LIMITS_HPP
#define NETWORK_SIN6_LENGTH_LIMITS_HPP

#include "network/sin6-sizes.hpp"       // sin6_size

namespace Network
{
    static constexpr auto sin6_length_max {sin6_size};
    static constexpr auto sin6_length_min {sin6_size};
}

#endif
