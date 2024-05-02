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

#include "network/close.h"                      // close()
#include "network/socket-null.h"                // socket_null
#include "network/socket-type.h"                // socket_type
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#ifdef WIN32
#include <winsock2.h>   // ::closesocket()
#else
#include <unistd.h>     // ::close()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::close(socket_type handle, bool verbose) -> OsErrorResult
{
#ifdef WIN32
    static constexpr const auto* method {"::closesocket"};
#else
    static constexpr const auto* method {"::close"};
#endif

    if (verbose) {
        std::cout << "Calling "
                  << method
                  << '('
                  << handle
                  << ')'
                  << std::endl;
    }

#ifdef WIN32
    const auto result {::closesocket(handle)};
#else
    const auto result {::close(handle)};
#endif

    if (result == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to "
            << method
            << '('
            << handle
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
