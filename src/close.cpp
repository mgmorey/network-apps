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
#include "network/close-function-name.h"        // close_function_name
#include "network/close-function-pointer.h"     // close_function_pointer
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-api-error.h"              // get_api_error()
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-api-error.h"            // reset_api_error()
#include "network/socket-error.h"               // socket_error
#include "network/socketdata.h"                 // SocketData
#include "network/to-os-error.h"                // to_os_error()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::close(const SocketData& sd) -> OsErrorResult
{
    if (sd.is_verbose()) {
        // clang-format off
        std::cout << "Calling "
                  << close_function_name
                  << '('
                  << sd.handle()
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();
    const auto result {close_function_pointer(sd.handle())};

    if (result == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to "
            << close_function_name
            << '('
            << sd.handle()
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
