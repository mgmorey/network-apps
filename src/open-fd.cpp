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

#include "network/open-fd.h"            // ByteString, Fd,
                                        // OpenHandler, OsErrorResult,
                                        // fd_type, open(),
                                        // operator<<()
#include "network/address.h"            // Address, operator<<()
#include "network/addresserror.h"       // AddressError
#include "network/get-length.h"         // get_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/is-valid.h"           // is_valid()
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/socket-error.h"       // socket_error

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::open(const OpenHandler& handler,
                   Fd fd,
                   const ByteString& str,
                   bool verbose) -> Network::OsErrorResult
{
    if (!is_valid(str)) {
        throw AddressError(str);
    }

    const auto length {get_length(str)};
    const auto *const pointer {get_sa_pointer(str)};
    reset_last_os_error();

    if (verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << fd
                  << ", "
                  << Address {str}
                  << ", "
                  << static_cast<int>(length)
                  << ')'
                  << std::endl;
    }

    const auto code {
        handler.first(static_cast<fd_type>(fd), pointer, length)
    };

    if (code == socket_error) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << fd
            << ", "
            << Address {str}
            << ") failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return OsErrorResult {};
}
