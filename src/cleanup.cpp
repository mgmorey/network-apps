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

#include "network/cleanup.h"            // cleanup()
#include "network/context-error.h"      // get_last_context_error(),
                                        // reset_last_context_error()
#include "network/context.h"            // Context
#include "network/exceptions.h"         // Error, LogicError,
                                        // RuntimeError
#include "network/format-os-error.h"    // format_os_error()
#include "network/socket-error.h"       // socket_error
#include "network/to-os-error.h"        // to_os_error()

#ifdef WIN32
#include <winsock2.h>       // WSAEINPROGRESS, WSAENETDOWN, WSANOTINITIALISED,
                            // ::WSACleanup()
#endif

auto Network::cleanup(Context::failure_mode t_mode) ->
    Network::context_error_type
{
#ifdef WIN32
    reset_last_context_error();

    if (::WSACleanup() == socket_error) {
        const auto error {get_last_context_error()};

        switch (t_mode) {
        case Context::failure_mode::throw_error:
        {
            const auto os_error {to_os_error(error)};
            const auto message {format_os_error(os_error)};

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
