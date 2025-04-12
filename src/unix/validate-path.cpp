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

#ifndef WIN32

#include "network/validate-path.hpp"            // validate()
#include "network/length-type.hpp"              // length_type
#include "network/path-length-limits.hpp"       // path_length_max,
                                                // path_length_min
#include "network/valueerror.hpp"               // ValueError

#include <string_view>  // std::string_view

auto Network::validate(std::string_view path) -> std::string_view
{
    const auto path_length {path.length()};

    if (path_length > path_length_max) {
        throw ValueError<length_type>("path_length_type",
                                      path_length,
                                      path_length_min,
                                      path_length_max);
    }

    return path;
}

#endif
