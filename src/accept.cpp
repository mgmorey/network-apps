// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/accept.h"             // accept()
#include "network/context-error.h"      // get_last_context_error(),
                                        // reset_last_context_error()
#include "network/descriptor-type.h"    // descriptor_type
#include "network/error.h"              // Error
#include "network/format-os-error.h"    // format_os_error()
#include "network/to-os-error.h"        // to_os_error()

#ifdef WIN32
#include <winsock2.h>       // ::accept()
#else
#include <sys/socket.h>     // ::accept()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::accept(const Socket& sock, bool verbose) -> Socket
{
    const auto handle_1 {descriptor_type {sock}};

    if (verbose) {
        std::cout << "Calling ::accept("
                  << handle_1
                  << ", ...)"
                  << std::endl;
    }

    reset_last_context_error();
    const auto handle_2 {::accept(handle_1, nullptr, nullptr)};

    if (handle_2 == descriptor_null) {
        const auto error = get_last_context_error();
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::accept("
            << handle_1
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        throw Error(oss.str());
    }

    return Socket {handle_2, false, verbose};
}
