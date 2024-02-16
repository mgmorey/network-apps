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

#include "network/to-path-bytestring.h"         // to_path()
#include "network/bytestring.h"                 // ByteString
#include "network/get-path-pointer-bs.h"        // get_path_pointer()
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/optionalpathname.h"           // OptionalPathname
#include "network/sun-len-limits.h"             // sun_len_min
#include "network/sun-offsets.h"                // sun_path_offset

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#endif

#include <cstring>      // ::strnlen()
#include <string>       // std::string

#ifndef WIN32

auto Network::to_path(const ByteString& addr) -> OptionalPathname
{
    if (get_sa_family(addr) != AF_UNIX || addr.size() <= sun_path_offset) {
        return {};
    }

    const auto* const data {get_path_pointer(addr)};
    const auto size_max {addr.size() - sun_len_min};
    const auto size {::strnlen(data, size_max)};
    return std::string {data, size};
}

#endif
