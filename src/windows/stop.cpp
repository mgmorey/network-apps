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

#include "network/stop.hpp"             // stop()
#include "network/error.hpp"            // Error
#include "network/failuremode.hpp"      // FailureMode
#include "network/format-os-error.hpp"  // format_os_error()
#include "network/get-api-error.hpp"    // get_api_error()
#include "network/logicerror.hpp"       // LogicError
#include "network/reset-api-error.hpp"  // reset_api_error()
#include "network/runtimeerror.hpp"     // RuntimeError
#include "network/socket-error.hpp"     // socket_error
#include "network/to-os-error.hpp"      // to_os_error()

#include <winsock2.h>       // WSAEINPROGRESS, WSAENETDOWN, WSANOTINITIALISED,
                            // ::WSACleanup()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::stop(FailureMode failure_mode, bool is_verbose) -> int
{
    reset_api_error();

    if (is_verbose) {
        // clang-format off
        std::cout << "Stopping the network runtime.\n"
                  << "Calling ::WSACleanup()"
                  << std::endl;
        // clang-format on
    }

    if (::WSACleanup() == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        const auto message {format_os_error(os_error)};

        if (is_verbose) {
            // clang-format off
            std::cout << "Call to ::WSACleanup() failed with error "
                      << api_error
                      << ": "
                      << message
                      << std::endl;
            // clang-format on
        }

        switch (failure_mode) {
        case FailureMode::throw_error:
        {
            switch (api_error) {  // NOLINT
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
            switch (api_error) {  // NOLINT
            case WSANOTINITIALISED:
                return 0;
            default:
                return api_error;
            }
            break;
        case FailureMode::return_error:
            return api_error;
        }
    }

    return 0;
}

#endif
