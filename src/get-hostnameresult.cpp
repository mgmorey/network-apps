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

#include "network/get-hostnameresult.h" // Hostname, HostnameResult,
                                        // OsErrorResult,
                                        // get_hostname(),
                                        // hostname_size_max
#include "network/buffer.h"             // Buffer
#include "network/os-error.h"           // format_os_error(),
                                        // get_os_error(),
                                        // reset_last_os_error()

#ifdef WIN32
#include <winsock2.h>   // ::gethostname()
#else
#include <unistd.h>     // ::gethostname()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_hostnameresult(bool verbose) noexcept -> Network::HostnameResult
{
    Buffer host_buffer {hostname_size_max};
    reset_last_os_error();

    if (verbose) {
        std::cout << "Calling ::gethostname("
                  << host_buffer
                  << ", "
                  << host_buffer.size() - 1
                  << ')'
                  << std::endl;
    }

    const auto size {static_cast<int>(host_buffer.size() - 1)};

    if (::gethostname(host_buffer.data(), size) == -1) {
        const auto error {get_last_os_error()};
        std::ostringstream oss;
        oss << "Call to ::gethostname("
            << host_buffer
            << ", "
            << host_buffer.size() - 1
            << ") failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return Hostname {host_buffer};
}
