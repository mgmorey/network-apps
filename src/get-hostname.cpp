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

#include "network/get-hostname.h"       // Hostname, HostnameResult,
                                        // get_hostname()
#include "network/buffer.h"             // Buffer
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/result.h"             // Result

#ifdef _WIN32
#include <winsock2.h>   // gethostname()
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#include <unistd.h>     // gethostname()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

auto Network::get_hostname(const Network::Hostname& host) ->
    Network::HostnameResult
{
    if (host.has_value()) {
        return host.value();
    }

    Buffer host_buffer {NI_MAXHOST};
    reset_last_os_error();

    if ((::gethostname(host_buffer.data(), host_buffer.size() - 1)) != 0) {
        auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to gethostname(...) failed with error "
            << error
            << ": "
            << format_os_error(error);
        return Result {error, oss.str()};
    }

    return std::string {host_buffer};
}
