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

#include "network/to-string-span-byte.h"        // to_string()

#include <cstddef>      // std::byte
#include <iomanip>      // std::setfill(), std::setw()
#include <ios>          // std::hex, std::ios, std::uppercase
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::to_string(const std::span<const std::byte>& span) -> std::string
{
    std::ostringstream oss;
    oss << "0x";

    if (span.empty()) {
        oss << '0';
    }
    else {
        oss << std::hex;

        for (const auto byte : span) {
            oss << std::setfill('0')
                << std::setw(2)
                << std::uppercase
                << static_cast<unsigned>(byte);
        }
    }

    return oss.str();
}
