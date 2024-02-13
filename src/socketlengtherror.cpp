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

#include "network/socketlengtherror.h"          // SocketLengthError
#include "network/rangeerror.h"                 // RangeError
#include "network/socket-length-limits.h"       // socket_length_max,
                                                // socket_length_min

#include <utility>      // std::move()
#include <version>

#ifdef __cpp_lib_format
#include <format>       // std::format()
#else
#include <sstream>      // std::ostringstream
#endif

#include <string>       // std::string

auto Network::SocketLengthError::format(const std::string& t_str) -> std::string
{
#ifdef __cpp_lib_format
    return std::format("Value {} is out of range [{}, {}] of socket_length_type",
                       t_str, socket_length_min, socket_length_max);
#else
    std::ostringstream oss;
    oss << "Value "
        << t_str
        << " is out of range ["
        << socket_length_min
        << ", "
        << socket_length_max
        << "] of socket_length_type";
    return oss.str();
#endif
}

Network::SocketLengthError::SocketLengthError(const std::string& t_str) noexcept :
    RangeError(format(t_str))
{
}

Network::SocketLengthError::SocketLengthError(std::string&& t_str) noexcept :
    RangeError(format(t_str))
{
    static_cast<void>(std::move(t_str));
}
