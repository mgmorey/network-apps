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

#include "network/startup.h"            // Context, Version, WSADATA,
                                        // WSAEFAULT, WSAEPROCLIM,
                                        // WSASYSNOTREADY,
                                        // WSAVERNOTSUPPORTED,
                                        // WSAStartup(), startup()
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/os-error.h"           // format_os_error()

#ifdef WIN32
static constexpr auto version_default {Network::Version {2, 2}};
#else
static constexpr auto version_default {Network::Version {0, 0}};
#endif

auto Network::startup(Context& context, const OptionalVersion& version) -> void
{
#ifdef WIN32
    WSADATA wsa_data {};
    const auto version_required {to_integer(version.value_or(version_default))};
    const auto error_code {::WSAStartup(version_required, &wsa_data)};

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

    context.description(static_cast<const char*>(wsa_data.szDescription))
        .system_status(static_cast<const char*>(wsa_data.szSystemStatus))
        .version(Version {to_version(wsa_data.wVersion)});
#else
    context.description("Berkeley Software Distribution Sockets")
        .version(version.value_or(version_default));
#endif
    context.is_started(true);
}
