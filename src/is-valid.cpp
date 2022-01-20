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

#include "network/is-valid.h"           // ByteString, is_valid()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-size.h"           // get_size_max(),
                                        // get_size_min()
#include "network/get-sun-length.h"     // get_sun_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/offsets.h"            // sun_path_offset
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/sizes.h"              // sin_size, sin6_size,
                                        // sockaddr_size_max, sun_size

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                            // sockaddr, sockaddr_in, sockaddr_storage
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                            // sockaddr, sockaddr_storage
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // std::memcpy(), std::memset()
#include <cstring>      // strnlen()
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::endl

auto Network::is_valid(const ByteString& addr, bool verbose) -> bool
{
    constexpr auto key_width {20};
    constexpr auto value_width {10};

    if (verbose) {
        std::cerr << "Validating socket address: "
                  << addr
                  << std::endl;
    }

    if (addr.empty()) {
        return false;
    }

    const auto family {get_sa_family(addr)};

    if (verbose) {
        std::cerr << std::setw(key_width) << "    Family: "
                  << std::right << std::setw(value_width) << family
                  << std::endl;
    }

    switch (family) {
    case AF_UNSPEC:
#ifndef _WIN32
    case AF_UNIX:
#endif
    case AF_INET:
    case AF_INET6:
        break;
    default:
        return false;
    }

    const auto addr_size {addr.size()};
    const auto addr_size_max {get_size_max(addr)};
    const auto addr_size_min {get_size_min(addr)};

    if (verbose) {
        std::cerr << std::setw(key_width) << "    Actual size: "
                  << std::right << std::setw(value_width) << addr_size
                  << std::endl
                  << std::setw(key_width) << "    Minimum size: "
                  << std::right << std::setw(value_width) << addr_size_min
                  << std::endl
                  << std::setw(key_width) << "    Maximum size: "
                  << std::right << std::setw(value_width) << addr_size_max
                  << std::endl;
    }

    if (!(addr_size_min <= addr_size && addr_size <= addr_size_max)) {
        return false;
    }

#ifdef HAVE_SOCKADDR_SA_LEN

    const auto sa_len {static_cast<std::size_t>(get_sa_length(addr))};

    if (verbose) {
        std::cerr << std::setw(key_width) << "    Stored length: "
                  << std::right << std::setw(value_width) << sa_len
                  << std::endl;
    }

    if (family == AF_UNIX) {
        if (!(addr_size_min <= sa_len && sa_len <= addr_size)) {
            return false;
        }

#ifndef _WIN32

        const auto *const sun {get_sun_pointer(addr)};
        const auto sun_len {get_sun_length(sun, addr_size)};

        if (verbose) {
            std::cerr << std::setw(key_width) << "    Computed length: "
                      << std::right << std::setw(value_width) << sun_len
                      << std::endl;
        }

        assert(sa_len == sun_len);  // NOLINT

#endif

    }
    else if (family == AF_INET || family == AF_INET6) {
        if (!(sa_len == addr_size)) {
            return false;
        }
    }

#endif

    return true;
}
