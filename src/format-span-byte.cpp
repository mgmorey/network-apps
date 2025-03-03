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

#include "network/format-span-byte.h"           // format()
#include "network/address.h"                    // Address
#include "network/logicerror.h"                 // LogicError
#include "network/to-string-span-byte.h"        // to_string()

#include <cstddef>      // std::byte
#include <optional>     // std::optional
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::format(const std::span<const std::byte>& bs) -> std::string
{
    try {
        std::ostringstream oss;
        oss << Address(bs);
        return oss.str();
    }
    catch (const LogicError& error) {
        return to_string(bs);
    }
}

auto Network::format(const std::span<const std::byte>& bs,
                     std::optional<std::string>& str) -> std::string
{
    if (!str) {
        str = format(bs);
    }

    return *str;
}
