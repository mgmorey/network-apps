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

#include "network/assert.h"             // assert()
#include "network/open-fd.h"            // Fd, OpenHandler,
                                        // OsErrorResult, open(),
                                        // operator<<()
#include "network/address.h"            // Address, operator<<()
#include "network/get-length.h"         // Bytes, get_length()
#include "network/get-sa-pointer.h"     // Bytes, get_sa_pointer()
#include "network/is-valid.h"           // Bytes, is_valid()
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/socket-error.h"       // socket_error

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::open(const OpenHandler& handler,
                   Fd fd,
                   const Bytes& addr,
                   bool verbose) -> Network::OsErrorResult
{
    assert(is_valid(addr, verbose));
    const auto addr_len {get_length(addr)};
    const auto *const addr_ptr {get_sa_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << fd
                  << ", "
                  << Address(addr)
                  << ", "
                  << static_cast<int>(addr_len)
                  << ')'
                  << std::endl;
    }

    reset_last_os_error();

    if (handler.first(static_cast<fd_type>(fd),
                      addr_ptr,
                      addr_len) == socket_error) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << fd
            << ", "
            << Address(addr)
            << ") failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return OsErrorResult {};
}
