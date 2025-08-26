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

#include "network/failmode.hpp"         // FailMode, operator<<()

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os, const FailMode& fm) -> std::ostream&
{
    switch (fm) {
    case FailMode::return_zero:
        os << "return zero";
        break;
    case FailMode::return_error:
        os << "return error";
        break;
    case FailMode::throw_error:
        os << "throw error";
        break;
    }

    return os;
}
