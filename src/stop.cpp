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

#include "network/stop.h"                       // stop()
#include "network/context-error-type.h"         // context_error_type
#include "network/context.h"                    // Context

#ifdef WIN32
#include "network/error.h"                      // Error
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/logicerror.h"                 // LogicError
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/runtimeerror.h"               // RuntimeError
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()
#endif

#ifdef WIN32
#include <winsock2.h>       // WSAEINPROGRESS, WSAENETDOWN, WSANOTINITIALISED,
                            // ::WSACleanup()
#endif

#ifdef WIN32
#include <iostream>     // std::cerr, std::cout, std::endl
#include <sstream>      // std::ostringstream
#endif

auto Network::stop(Context::failure_mode t_mode,
                   bool t_is_verbose) -> context_error_type
{
#ifdef WIN32
    reset_last_context_error();

    if (t_is_verbose) {
        std::cout << "Calling ::WSACleanup()"
                  << std::endl;
    }

    if (::WSACleanup() == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        const auto message {format_os_error(os_error)};

        if (t_is_verbose) {
            std::cerr << "Call to ::WSACleanup() failed with error "
                      << error
                      << ": "
                      << message
                      << std::endl;
        }

        switch (t_mode) {
        case Context::failure_mode::throw_error:
        {
            switch (error) {  // NOLINT
            case WSANOTINITIALISED:
                throw LogicError {message};
            case WSAEINPROGRESS:
            case WSAENETDOWN:
                throw RuntimeError {message};
            default:
                throw Error {message};
            }
            break;
        }
        case Context::failure_mode::return_zero:
            switch (error) {  // NOLINT
            case WSANOTINITIALISED:
                return 0;
            default:
                return error;
            }
            break;
        case Context::failure_mode::return_error:
            return error;
        }
    }

#else
    static_cast<void>(t_mode);
#endif
    return 0;
}
