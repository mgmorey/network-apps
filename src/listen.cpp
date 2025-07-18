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

#include "network/listen.hpp"           // listen()
#include "network/format-os-error.hpp"  // format_os_error()
#include "network/get-api-error.hpp"    // get_api_error()
#include "network/oserror.hpp"          // OsError
#include "network/reset-api-error.hpp"  // reset_api_error()
#include "network/socket-error.hpp"     // socket_error
#include "network/socketcore.hpp"       // SocketCore
#include "network/to-os-error.hpp"      // to_os_error()

#ifdef _WIN32
#include <winsock2.h>       // ::listen()
#else
#include <sys/socket.h>     // ::listen()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::listen(const SocketCore& sc, int backlog) -> OsError
{
    const auto handle {sc.handle()};

    if (sc.runtime()->is_verbose()) {
        // clang-format off
        std::cout << "Calling ::listen("
                  << handle
                  << ", "
                  << backlog
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();
    const auto result {::listen(handle, backlog)};

    if (result == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::listen("
            << handle
            << ", "
            << backlog
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return {os_error, oss.str()};
    }

    return {};
}
