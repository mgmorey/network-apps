// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/runtime.hpp"                  // Runtime
#include "network/string-null.hpp"              // string_null
#include "network/to-string-runtime.hpp"        // to_string()

#include <sstream>      // std::ostringstream
#include <string>       // std::string

extern auto Network::to_string(const Runtime& rt) -> std::string
{
    std::ostringstream oss;
    const auto start {oss.tellp()};
    oss << rt.description();

    if (static_cast<bool>(rt.version())) {
        if (oss.tellp() != start) {
            oss << ' ';
        }

        oss << rt.version();
    }

#ifdef CODE_COVERAGE_ENABLED

    if (oss.tellp() != start) {
        oss << ' ';
    }

    oss << "(with code coverage enabled)";

#endif

    if (oss.tellp() != start) {
        oss << ' ';
    }

    oss << (rt.system_status().empty() ? string_null : rt.system_status());
    return oss.str();
}
