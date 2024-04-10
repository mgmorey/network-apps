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

#include "network/validate-char.h"              // validate()
#include "network/path-length-limits.h"         // path_length_max,
                                                // path_length_min
#include "network/pathlengtherror.h"            // PathLengthError

#include <cstring>      // ::strnlen()
#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

auto Network::validate(const char* path) -> void
{
    if (path == nullptr) {
        return;
    }

    const auto path_length {::strnlen(path, path_length_max)};

    if (std::cmp_less(path_length, path_length_min) ||
        std::cmp_greater(path_length, path_length_max)) {
        throw PathLengthError(std::to_string(path_length));
    }
}

#endif
