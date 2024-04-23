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
#include "network/path-length-limits.h"         // path_length_max,
                                                // path_length_min
#include "network/path-length-type.h"           // path_length_type
#include "network/valueerror.h"                 // ValueError

auto Network::validate(const OptionalPathname& path) -> void
{
    if (!path) {
        return;
    }

    const auto path_length {path->length()};

    if (path_length > path_length_max) {
        throw ValueError<path_length_type>("path_length_type",
                                           path_length,
                                           path_length_min,
                                           path_length_max);
    }
}

#endif
