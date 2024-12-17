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

#ifdef WIN32

#include "network/stop.h"                       // stop()
#include "network/failuremode.h"                // FailureMode

#include "network/error.h"                      // Error
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/logicerror.h"                 // LogicError
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/runtimeerror.h"               // RuntimeError
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#include <winsock2.h>       // WSAEINPROGRESS, WSAENETDOWN, WSANOTINITIALISED,
                            // ::WSACleanup()

#include <iostream>     // std::cerr, std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::stop(FailureMode mode, bool is_verbose) -> int
{
    reset_last_context_error();

    if (is_verbose) {
        std::cout << "Stopping the network runtime.\n"
                  << "Calling ::WSACleanup()"
                  << std::endl;
    }

    if (::WSACleanup() == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        const auto message {format_os_error(os_error)};

        if (is_verbose) {
            std::cerr << "Call to ::WSACleanup() failed with error "
                      << error
                      << ": "
                      << message
                      << std::endl;
        }

        switch (mode) {
        case FailureMode::throw_error:
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
        case FailureMode::return_zero:
            switch (error) {  // NOLINT
            case WSANOTINITIALISED:
                return 0;
            default:
                return error;
            }
            break;
        case FailureMode::return_error:
            return error;
        }
    }

    return 0;
}

#endif
