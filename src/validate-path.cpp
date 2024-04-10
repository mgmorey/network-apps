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

#include "network/validate-path.h"              // validate()
#include "network/path-length-limits.h"         // path_length_max
#include "network/pathlengtherror.h"            // PathLengthError

#include <string>       // std::to_string()

auto Network::validate(const OptionalPathname& path) -> void
{
    if (!path) {
        return;
    }

    const auto path_length {path->length()};

    if (path_length > path_length_max) {
        throw PathLengthError(std::to_string(path_length));
    }
}

#endif
