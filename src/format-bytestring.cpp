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

#include "network/format-bytestring.h"          // format()
#include "network/address.h"                    // Address
#include "network/addresserror.h"               // AddressError
#include "network/bytestring.h"                 // ByteString
#include "network/optionalstring.h"             // OptionalString
#include "network/to-hex.h"                     // to_hex()

#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::format(const ByteString& addr) noexcept -> std::string
{
    try {
        std::ostringstream oss;
        oss << Address(addr);
        return oss.str();
    }
    catch (const AddressError& error) {
        return to_hex(addr);
    }
}

auto Network::format(const ByteString& addr,
                     OptionalString& addr_str) noexcept -> std::string
{
    if (!addr_str) {
        addr_str = format(addr);
    }

    return *addr_str;
}
