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

#include <cstddef>      // std::byte
#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <tuple>        // std::get()
#include <utility>      // std::cmp_equal()

auto Network::open(const SocketData& sd,
                   std::span<const std::byte> bs,
                   const OpenHandler& oh) -> OsErrorResult
{
    const auto [sa, sa_length] {get_sa_span(bs)};
    const auto handle {sd.handle()};
    const auto is_verbose {sd.runtime()->is_verbose()};

    if (std::cmp_equal(sa_length, sa_length_min)) {
        throw AddressError("Address payload length is zero: " +
                           to_string(bs));
    }

    if (is_verbose) {
        // clang-format off
        std::cout << "Calling "
                  << std::get<1>(oh)
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

    if (std::get<0>(oh)(handle, sa, sa_length) == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to "
            << std::get<1>(oh)
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
