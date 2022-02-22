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
#include "network/context.h"            // Context
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/socket-error.h"       // socket_error

#ifdef WIN32
#include <winsock2.h>       // WSAEINPROGRESS, WSAENETDOWN, WSANOTINITIALISED,
                            // ::WSACleanup()
#endif

auto Network::cleanup(Context::failure_mode t_mode) -> Network::os_error_type
{
#ifdef WIN32
    reset_last_os_error();

    if (::WSACleanup() == socket_error) {
        const auto error_code {get_last_os_error()};

        if (t_mode == Context::failure_mode::throw_error) {
            const auto error_str {format_os_error(error_code)};

            switch (error_code) {  // NOLINT
            case WSANOTINITIALISED:
                throw LogicError {error_str};
                break;
            case WSAEINPROGRESS:
            case WSAENETDOWN:
                throw RuntimeError {error_str};
                break;
            default:
                throw Error {error_str};
            }
        }
        else if (t_mode == Context::failure_mode::return_zero) {
            switch (error_code) {  // NOLINT
            case WSANOTINITIALISED:
                return 0;
                break;
            }
        }

        return error_code;
    }

#else
    static_cast<void>(t_mode);
#endif
    return 0;
}
