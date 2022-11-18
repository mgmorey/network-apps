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

#include "network/sizeerror.h"          // Error, SizeError

#include <climits>      // SIZE_MAX
#if __has_include(<format>)
#include <format>       // std::format()
#else
#include <sstream>      // std::ostringstream
#endif

auto Network::SizeError::format(const std::string& t_value) -> std::string
{
#ifdef __cpp_lib_format
    return std::format("Value {} is out of range [{}, {}] of std::size_t",
                       t_value, 0, SIZE_MAX);
#else
    std::ostringstream oss;
    oss << "Value "
        << t_value
        << " is out of range ["
        << 0
        << ", "
        << SIZE_MAX
        << "] of std::size_t";
    return oss.str();
#endif
}

Network::SizeError::SizeError(const std::string& t_value) noexcept :
    RangeError(format(t_value))
{
}

Network::SizeError::SizeError(std::string&& t_value) noexcept :
    RangeError(format(t_value))
{
}
