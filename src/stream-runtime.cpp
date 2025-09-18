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

#include "network/runtime.hpp"          // Runtime, operator<<()

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os, const Runtime& rt) -> std::ostream&
{
    const auto start {os.tellp()};
    os << rt.description();

    if (static_cast<bool>(rt.version())) {
        if (os.tellp() != start) {
            os << ' ';
        }

        os << rt.version();
    }

#ifdef CODE_COVERAGE_ENABLED

    if (os.tellp() != start) {
        os << ' ';
    }

    os << "(with code coverage enabled)";

#endif

    if (os.tellp() != start) {
        os << ' ';
    }

    os << rt.system_status();
    return os;
}
