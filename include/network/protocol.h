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

#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "network/family.h"             // Family, family_type
#include "network/integer.h"            // Integer
#include "network/protocol-type.h"      // protocol_type

#include <ostream>      // std::ostream

namespace Network
{
    class Protocol :
        public Integer<protocol_type>
    {
    public:
        constexpr Protocol(family_type t_family,
                           protocol_type t_value) noexcept :
            Integer(t_value),
            m_family(t_family)
        {
        }

        [[nodiscard]] constexpr auto family() const noexcept -> Family
        {
            return m_family;
        }

    private:
        Family m_family;
    };

    extern auto operator<<(std::ostream& os,
                           const Protocol& protocol) noexcept -> std::ostream&;
}

#endif
