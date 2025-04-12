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

#ifndef WIN32

#include "network/get-sun-pointer.hpp"  // get_sun_pointer()
#include "network/validate-sun.hpp"     // validate()

#include <sys/un.h>         // sockaddr_un

#include <cstddef>      // std::byte
#include <span>         // std::span

auto Network::get_sun_pointer(std::span<const std::byte> bs) ->
    const sockaddr_un*
{
    const void* pointer {bs.data()};
    return validate(static_cast<const sockaddr_un*>(pointer), bs.size());
}

#endif
