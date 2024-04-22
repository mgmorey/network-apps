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

#include "network/rangeerror.h"         // RangeError
#include "network/logicerror.h"         // LogicError

#include <string>       // std::string
#include <utility>      // std::move()

#include <version>

#ifdef __cpp_lib_format
#include <format>       // std::format()
#else
#include <sstream>      // std::ostringstream
#endif

auto Network::RangeError::format(const std::string& t_value,
                                 const std::string& t_min,
                                 const std::string& t_max,
                                 const std::string& t_type) -> std::string
{
#ifdef __cpp_lib_format
    return std::format("Value {} is out of range [{}, {}] of {}",
                       t_value, t_min, t_max, t_type);
#else
    std::ostringstream oss;
    oss << "Value "
        << t_value
        << " is out of range ["
        << t_min
        << ", "
        << t_max
        << "] of "
        << t_type;
    return oss.str();
#endif
}

Network::RangeError::RangeError(const std::string& t_str) noexcept :
    LogicError(t_str)
{
}


Network::RangeError::RangeError(std::string&& t_str) noexcept :
    LogicError(t_str)
{
    static_cast<void>(std::move(t_str));
}

Network::RangeError::RangeError(const std::string& t_value,
                                const std::string& t_min,
                                const std::string& t_max,
                                const std::string& t_type) noexcept :
    LogicError(format(t_value, t_min, t_max, t_type))
{
}
