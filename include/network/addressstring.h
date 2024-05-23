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

#ifndef NETWORK_ADDRESSSTRING_H
#define NETWORK_ADDRESSSTRING_H

#include "network/bytestring.h"         // ByteString

#include <optional>     // std::optional
#include <string>       // std::string

namespace Network
{
    class AddressString
    {
        friend auto operator<<(std::ostream& os,
                               const AddressString& as) -> std::ostream&;

    public:
        explicit AddressString(const ByteString& t_addr);
        AddressString(const AddressString&) = default;
        AddressString(AddressString&&) = default;
        ~AddressString() = default;
        auto operator=(const AddressString&) -> AddressString& = default;
        auto operator=(AddressString&&) -> AddressString& = default;
        explicit operator std::string() const;

    private:
        ByteString m_addr;
        mutable std::optional<std::string> m_addr_str;
    };

    extern auto operator<<(std::ostream& os,
                           const AddressString& as) -> std::ostream&;
}

#endif
