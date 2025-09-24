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

#ifndef NETWORK_GET_HOSTNAME_BOOL_HPP
#define NETWORK_GET_HOSTNAME_BOOL_HPP

#include "network/hostname.hpp"         // Hostname
#include "network/sharedruntime.hpp"    // SharedRuntime

namespace Network
{
    extern auto get_hostname(const SharedRuntime& sr) -> Hostname;
    extern auto get_hostname(bool is_verbose = false) -> Hostname;
}

#endif
