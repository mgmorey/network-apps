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

#ifndef WIN32

#include "network/read.h"                       // read()
#include "network/error.h"                      // Error
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/handle-type.h"                // handle_type
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#include <sys/types.h>          // ssize_t

#include <unistd.h>         // ::read()

#include <cstddef>      // std::size_t
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::read(handle_type handle,
                   char* data,
                   std::size_t size,
                   bool is_verbose) -> ssize_t
{
    if (is_verbose) {
        std::cout << "Calling ::read("
                  << handle
                  << ", \""
                  << data
                  << "\", "
                  << size
                  << ')'
                  << std::endl;
    }

    reset_last_context_error();
    const auto result {::read(handle, data, size)};

    if (result == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::read("
            << handle
            << ", \""
            << data
            << "\", "
            << size
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        throw Error(oss.str());
    }

    if (is_verbose) {
        std::cout << "Call to ::read("
                  << handle
                  << ", \""
                  << data
                  << "\", "
                  << size << ") returned data \""
                  << data
                  << '\"'
                  << std::endl;
    }

    return result;
}

#endif
