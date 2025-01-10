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

#include "network/shutdown.h"                   // shutdown()
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/handle-type.h"                // handle_type
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#ifdef WIN32
#include <winsock2.h>       // ::shutdown()
#else
#include <sys/socket.h>     // ::shutdown()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::shutdown(handle_type handle,
                       int how,
                       bool is_verbose) -> OsErrorResult
{
    if (is_verbose) {
        std::cout << "Calling ::shutdown("
                  << handle
                  << ')'
                  << std::endl;
    }

    reset_last_context_error();
    const auto result {::shutdown(handle, how)};

    if (result == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::shutdown("
            << handle
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
