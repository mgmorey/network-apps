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

#ifndef NETWORK_FAMILY_H
#define NETWORK_FAMILY_H

#include "network/family-type.h"        // family_type
#include "network/integer.h"            // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Family :
        public Integer<family_type>
    {
        friend auto operator<<(std::ostream& os,
                               const Family& family) -> std::ostream&;

    public:
        explicit Family(family_type t_value);
    };

    extern auto operator<<(std::ostream& os,
                           const Family& family) -> std::ostream&;
}

#endif
