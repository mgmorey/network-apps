// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/open-handle.hpp"              // open()
#include "network/addresserror.hpp"             // AddressError
#include "network/format-os-error.hpp"          // format_os_error()
#include "network/get-api-error.hpp"            // get_api_error()
#include "network/get-sa-span.hpp"              // get_sa_span()
#include "network/openhandler.hpp"              // OpenHandler
#include "network/oserrorresult.hpp"            // OsErrorResult
#include "network/reset-api-error.hpp"          // reset_api_error()
#include "network/sa-length-limits.hpp"         // sa_length_min
#include "network/socket-error.hpp"             // socket_error
#include "network/socketdata.hpp"               // SocketData
#include "network/to-os-error.hpp"              // to_os_error()
#include "network/to-string-span-byte.hpp"      // to_string()

#ifdef WIN32
#include <winsock2.h>       // ::bind(), ::connect()
#else
#include <sys/socket.h>     // ::bind(), ::connect()
#endif

#include <array>        // std::arrray
#include <cstddef>      // std::byte, std::size_t
#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <utility>      // std::cmp_equal(), std::make_pair

namespace
{
    auto get_binding(bool is_bind) -> Network::OpenHandler
    {
        static const std::array<Network::OpenHandler, 2> bindings {
            std::make_pair(::connect, "::connect"),
            std::make_pair(::bind, "::bind"),
        };

        return bindings.at(static_cast<std::size_t>(is_bind));
    }
}

auto Network::open(const SocketData& sd,
                   std::span<const std::byte> bs,
                   bool is_bind) -> OsErrorResult
{
    const auto binding {get_binding(is_bind)};
    const auto [sa, sa_length] {get_sa_span(bs)};
    const auto handle {sd.handle()};

    if (std::cmp_equal(sa_length, sa_length_min)) {
        throw AddressError("Address payload length is zero: " +
                           to_string(bs));
    }

    if (sd.is_verbose()) {
        // clang-format off
        std::cout << "Calling "
                  << binding.second
                  << '('
                  << handle
                  << ", "
                  << to_string(bs)
                  << ", "
                  << sa_length
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();

    if (binding.first(handle, sa, sa_length) == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to "
            << binding.second
            << '('
            << handle
            << ", "
            << to_string(bs)
            << ", "
            << sa_length
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
