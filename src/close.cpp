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
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-close-function-name.h"    // get_close_function_name()
#include "network/get-close-function-pointer.h" // get_close_function_pointer()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/handle-type.h"                // handle_type
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::close(handle_type handle, bool is_verbose) -> OsErrorResult
{
    const char* function_name {get_close_function_name};
    const CloseFunctionPointer function_pointer {get_close_function_pointer()};

    if (is_verbose) {
        std::cout << "Calling "
                  << function_name
                  << '('
                  << handle
                  << ')'
                  << std::endl;
    }

    reset_last_context_error();
    const auto result {function_pointer(handle)};

    if (result == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to "
            << function_name
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
