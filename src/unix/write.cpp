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

#include "network/write.h"              // write()
#include "network/error.h"              // Error
#include "network/format-os-error.h"    // format_os_error()
#include "network/get-api-error.h"      // get_api_error()
#include "network/handle-type.h"        // handle_type
#include "network/reset-api-error.h"    // reset_api_error()
#include "network/socket-error.h"       // socket_error
#include "network/to-os-error.h"        // to_os_error()

#include <sys/types.h>          // ssize_t

#include <unistd.h>         // ::write()

#include <cstddef>      // std::size_t
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::write(handle_type handle,
                    const char* data,
                    std::size_t size,
                    bool is_verbose) -> ssize_t
{
    if (is_verbose) {
        std::cout << "Calling ::write("
                  << handle
                  << ", \""
                  << data
                  << "\", "
                  << size
                  << ')'
                  << std::endl;
    }

    reset_api_error();
    const auto result {::write(handle, data, size)};

    if (result == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        oss << "Call to ::write("
            << handle
            << ", \""
            << data
            << "\", "
            << size
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        throw Error(oss.str());
    }

    return result;
}

#endif
