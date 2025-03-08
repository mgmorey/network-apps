// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifdef WIN32

#include "network/read.h"               // read()
#include "network/error.h"              // Error
#include "network/format-os-error.h"    // format_os_error()
#include "network/get-api-error.h"      // get_api_error()
#include "network/quote.h"              // quote()
#include "network/reset-api-error.h"    // reset_api_error()
#include "network/socket-error.h"       // socket_error
#include "network/socketdata.h"         // SocketData
#include "network/to-os-error.h"        // to_os_error()

#include <sys/types.h>      // ssize_t
#include <winsock2.h>       // ::recv()

#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

auto Network::read(const SocketData& sd, std::span<char> sc) -> ssize_t
{
    const std::string_view sv {sc.data(), sc.size()};
    const auto handle {sd.handle()};
    const auto is_verbose {sd.is_verbose()};

    if (is_verbose) {
        // clang-format off
        std::cout << "Calling ::recv("
                  << handle
                  << ", "
                  << quote(sv)
                  << ", "
                  << sc.size()
                  << ", 0)"
                  << std::endl;
        // clang-format on
    }

    reset_api_error();
    const auto result {::recv(handle, sc.data(), static_cast<int>(sc.size()), 0)};

    if (result == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::recv("
            << handle
            << ", "
            << quote(sv)
            << ", "
            << sc.size()
            << ", 0) failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        throw Error(oss.str());
    }

    if (is_verbose) {
        // clang-format off
        std::cout << "Call to ::recv("
                  << handle
                  << ", "
                  << quote(sv)
                  << ", "
                  << sc.size()
                  << ", 0) returned data "
                  << quote(sv)
                  << std::endl;
        // clang-format on
    }

    return result;
}

#endif
