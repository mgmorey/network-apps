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
    if (rt.system_status().empty()) {
        return os;
    }

    os << rt.description();

    if (static_cast<bool>(rt.version())) {
        if (os.tellp() != 0) {
            os << ' ';
        }

        os << "Version " << rt.version();
    }

    if (os.tellp() != 0) {
        os << ' ';
    }

    os << rt.system_status();

#ifdef CODE_COVERAGE_ENABLED
    if (os.tellp() != 0) {
        os << ' ';
    }

    os << "(with code coverage enabled)";
#endif

    return os;
}
