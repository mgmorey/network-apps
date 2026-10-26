// Copyright (C) 2026  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SSIZE_TYPE_HPP
#define NETWORK_SSIZE_TYPE_HPP

#include <cstddef>      // std::ptrdiff_t

#ifndef _WIN32
#include <sys/types.h>  // ssize_t
#endif

namespace Network
{
#ifdef _WIN32
    using ssize_type = std::ptrdiff_t;
#else
    using ssize_type = ssize_t;
#endif
}

#endif
