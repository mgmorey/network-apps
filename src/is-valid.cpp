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
#include "network/get-sa-size.h"        // get_sa_size_maximum(),
                                        // get_sa_size_minimum()
#include "network/get-sun-length.h"     // get_sun_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cout, std::endl

auto Network::is_valid(const ByteString& addr, bool verbose) noexcept -> bool
{
    constexpr auto key_width {20};
    constexpr auto value_width {10};

    if (verbose) {
        std::cout << "Validating socket address: "
                  << addr
                  << std::endl;
    }

    if (addr.empty()) {
        return false;
    }

    const auto family {get_sa_family(addr)};

    if (verbose) {
        std::cout << std::setw(key_width) << "    Family: "
                  << std::right << std::setw(value_width) << family
                  << std::endl;
    }

    switch (family) {
    case AF_UNSPEC:
#ifndef WIN32
    case AF_UNIX:
#endif
    case AF_INET:
    case AF_INET6:
        break;
    default:
        return false;
    }

    const auto addr_size {addr.size()};
    const auto addr_size_max {get_sa_size_maximum(family)};
    const auto addr_size_min {get_sa_size_minimum(family)};

    if (verbose) {
        std::cout << std::setw(key_width) << "    Actual size: "
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

    const auto sa_len {get_sa_length(addr)};

    if (verbose) {
        std::cout << std::setw(key_width) << "    Stored length: "
                  << std::right << std::setw(value_width) << sa_len
                  << std::endl;
    }

    if (family == AF_UNIX) {
        if (!(addr_size_min <= sa_len && sa_len <= addr_size)) {
            return false;
        }

#ifndef WIN32
        const auto* const sun {get_sun_pointer(addr)};
        const auto sun_len {get_sun_length(sun, addr_size)};

        if (verbose) {
            std::cout << std::setw(key_width) << "    Computed length: "
                      << std::right << std::setw(value_width) << sun_len
                      << std::endl;
        }
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
