// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/shutdown.hpp"         // shutdown()
#include "network/format-os-error.hpp"  // format_os_error()
#include "network/get-api-error.hpp"    // get_api_error()
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/reset-api-error.hpp"  // reset_api_error()
#include "network/socket-error.hpp"     // socket_error
#include "network/socketdata.hpp"       // SocketData
#include "network/to-os-error.hpp"      // to_os_error()

#ifdef WIN32
#include <winsock2.h>       // ::shutdown()
#else
#include <sys/socket.h>     // ::shutdown()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::shutdown(const SocketData& sd, int how) -> OsErrorResult
{
    const auto handle {sd.handle()};

    if (sd.runtime()->is_verbose()) {
        // clang-format off
        std::cout << "Calling ::shutdown("
                  << handle
                  << ", "
                  << how
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();
    const auto result {::shutdown(handle, how)};

    if (result == socket_error) {
        const auto error {get_api_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::shutdown("
            << handle
            << ", "
            << how
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
