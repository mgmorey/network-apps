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

#ifndef NETWORK_GET_HOSTNAMERESULT_H
#define NETWORK_GET_HOSTNAMERESULT_H

#include "network/hostnameresult.h"             // HostnameResult
#include "network/oserrorresult.h"              // OsErrorResult

#include <span>         // std::span

namespace Network
{
    extern auto get_hostnameresult(const std::span<char>& hostname,
                                   bool is_verbose = false) -> OsErrorResult;
    extern auto get_hostnameresult(bool is_verbose = false) -> HostnameResult;
}

#endif
