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

#ifndef NETWORK_OVERLOADED_H
#define NETWORK_OVERLOADED_H

namespace Network
{
    // The following are based upon examples provided in section
    // 13.5.1 of "A Tour of C++" (Second Edition) by Bjarne
    // Stroustrup.

    template <typename... Ts>
    struct Overloaded : Ts... {
        using Ts::operator()...;
    };

    template <typename... Ts>
    Overloaded(Ts...) -> Overloaded<Ts...>; // deduction guide
}

#endif
