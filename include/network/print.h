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

#ifndef NETWORK_PRINT_H
#define NETWORK_PRINT_H

#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase
#include <ostream>      // std::ios, std::ostream

namespace Network
{
    template<typename Container>
    auto print(std::ostream& os, const Container& value) -> std::ostream&
    {
        std::ios format {nullptr};
        format.copyfmt(os);
        os << "0x";

        if (value.empty()) {
            os << '0';
        }
        else {
            os << std::hex;

            for (const auto by : value) {
                os << std::setfill('0')
                   << std::setw(2)
                   << std::uppercase
                   << static_cast<int>(by);
            }
        }

        os.copyfmt(format);
        return os;
    }
}

#endif
