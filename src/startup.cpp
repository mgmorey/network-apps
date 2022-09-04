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

#include "network/startup.h"            // Context, Version, startup()
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/os-error.h"           // format_os_error()

#ifdef WIN32
#include <winsock2.h>       // WSADATA, WSAEFAULT, WSAEPROCLIM,
                            // WSASYSNOTREADY, WSAVERNOTSUPPORTED,
                            // ::WSAStartup()
#endif

#ifdef WIN32
static constexpr auto api_default {Network::Version {2, 2}};
#else
static constexpr auto api_default {Network::Version {}};
#endif

auto Network::startup(Context& context, const OptionalVersion& version) -> void
{
    const auto api_version {version.value_or(api_default)};
#ifdef WIN32
    WSADATA data {};
    const auto error_code {::WSAStartup(api_version.value(), &data)};

    if (error_code != 0) {
        const auto error_str {format_os_error(error_code)};

        switch (error_code) {  // NOLINT
        case WSAEFAULT:
        case WSAVERNOTSUPPORTED:
            throw LogicError {error_str};
            break;
        case WSAEPROCLIM:
        case WSASYSNOTREADY:
            throw RuntimeError {error_str};
            break;
        default:
            throw Error {error_str};
        }
    }

    context.description(static_cast<const char*>(data.szDescription))
        .system_status(static_cast<const char*>(data.szSystemStatus))
        .version(Version {data.wVersion});
#else
    context.description("Berkeley Software Distribution Sockets")
        .system_status("")
        .version(api_version);
#endif
    context.is_started(true);
}
