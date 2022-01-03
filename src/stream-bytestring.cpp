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

#include "network/bytestring.h"         // Byte, ByteString,
                                        // operator<<(), std::ios,
                                        // std::ostream
#include "network/to-integer.h"         // to_integer()

#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase

auto Network::operator<<(std::ostream& os,
                         const ByteString& bytes) -> std::ostream&
{
    std::ios format {nullptr};
    format.copyfmt(os);
    os << "0x";

    if (bytes.empty()) {
        os << '0';
    }
    else {
        os << std::hex;

        for (const auto byte : bytes) {
            os << std::setfill('0')
               << std::setw(2)
               << std::uppercase
               << Network::to_integer<unsigned>(byte);
        }
    }

    os.copyfmt(format);
    return os;
}
