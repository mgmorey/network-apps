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

#ifndef NETWORK_GET_ENDPOINTRESULT_HPP
#define NETWORK_GET_ENDPOINTRESULT_HPP

#include "network/endpointresult.hpp"   // EndpointResult
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/sharedruntime.hpp"    // SharedRuntime

#include <cstddef>      // std::byte
#include <span>         // std::span

namespace Network
{
    extern auto get_endpointresult(std::span<char> hostname,
                                   std::span<char> service,
                                   std::span<const std::byte> bs, int flags,
                                   const SharedRuntime& sr) -> OsErrorResult;
    extern auto get_endpointresult(std::span<const std::byte> bs, int flags,
                                   const SharedRuntime& sr) -> EndpointResult;
    extern auto get_endpointresult(std::span<const std::byte> bs, int flags,
                                   bool is_verbose = false) -> EndpointResult;
}

#endif
