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
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/format-os-error.hpp"          // format_os_error()
#include "network/get-api-error.hpp"            // get_api_error()
#include "network/get-openhandler.hpp"          // get_openhandler()
#include "network/get-sa-span.hpp"              // get_sa_span()
#include "network/opensymbol.hpp"               // OpenSymbol
#include "network/oserror.hpp"                  // OsError
#include "network/reset-api-error.hpp"          // reset_api_error()
#include "network/sa-length-limits.hpp"         // sa_length_min
#include "network/socket-error.hpp"             // socket_error
#include "network/socketcore.hpp"               // SocketCore
#include "network/to-os-error.hpp"              // to_os_error()
#include "network/to-string-span-byte.hpp"      // to_string()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <utility>      // std::cmp_equal()

auto Network::open(const SocketCore& sc, ByteSpan bs, OpenSymbol symbol) -> OsError
{
    const auto [sa, sa_length] {get_sa_span(bs)};
    const auto handle {sc.handle()};
    const auto oh {get_openhandler(symbol)};

    if (std::cmp_equal(sa_length, sa_length_min)) {
        throw AddressError("Address payload length is zero: " +
                           to_string(bs));
    }

    if (sc.runtime()->is_verbose()) {
        // clang-format off
        std::cout << "Calling "
                  << oh.string()
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

    if (oh.function()(handle, sa, sa_length) == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to "
            << oh.string()
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
        return {os_error, oss.str()};
    }

    return {};
}
